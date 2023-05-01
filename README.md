# 🐚 minishell 🐚
* 나만의 mini bash shell 만들기
* following builtins
  * echo with option -n
  * cd with only a relative or absolute path
  * pwd with no options
  * export with no options
  * unset with no options
  * env with no options or arguments
  * exit with no options
* <<, <, >, >>, |, "", '' 등 주의해서 짜기

## ☘️ 허용함수 ☘️
 
### fork()

fork()는 현재 실행중인 process를 복사해서 다른 process를 생성한다. 복사해서 생성하기 때문에, 가지고 있던 메모리등의 시스템 자원을 모두 원래의 process와 공유하게 된다.

`fork()`를 사용하여 생성한 프로세스는 **부모 프로세스 Parent process**, 새로 생긴 프로세스는 **자식 프로세스 Child process** 라고 부른다.

모든 프로세스는 (참고: 최상위 프로세스인 `init`는 pid 1을 가진다) 생성될 때 프로세스 아이디를 부여받는다. `fork()` 함수는 부모에게는 *자식 프로세스의 pid*를 반환하고, 자식에게는 *0*을 반환한다. 이를 이용하여 자식 프로세스에게 특정 명령을 시킬 수 있다.

### wait, waitpid, wait3, wait4

fork 함수로 자식 프로세스를 생성하면 **부모 프로세스와 자식 프로세스는 순서에 관계 없이 실행**되고, 먼저 실행을 마친 프로세스는 종료한다. 이때 좀비 프로세스(zombie procss)같은 불안정 상태의 프로세스가 발생하는데 이를 방지하려면 프로세스 동기화 함수를 수행해서 부모 프로세스와 자식 프로세스를 동기화 시켜야한다.

프로세스 동기화 함수로 사용하는 것이 wait 계열 함수이다.

| 함수 원형 | 기능 |
| --- | --- |
| pid_t wait(int *stat_loc); | 임의의 자식 프로세스의 상태값 구하기 |
| pid_t waitpid(pid_t pid, int *stat_loc, int options); | 특정 프로세스의 상태값 구하기 |

동기화 후에는 자식 프로세스는 `if(pid == 0)`일 경우의 구문을 수행한 뒤 종료하며, 부모 프로세스의 경우엔 wait()를 통하여 자식 프로세스가 종료된 뒤 나머지 구문을 수행한 뒤 종료한다.

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
		pid_t pid;

    pid = fork();
    if (pid == 0)
    {
        printf("자식 프로세스\n");
	      exit(0);
    }
    if (pid > 0)
    {
        printf("Wait\n");
        wait(NULL);
        printf("Exit\n");
    }
  return 0;
}
```

### signal()

```c
sig_t signal(int sig, sig_t func);
```

sig 는 시그널 번호, func 는 해당 시그널을 처리할 핸들러.

- Signal이란 Software interrupt로, process에 무엇인가 발생했음을 알리는 간단한 메시지를 비동기적으로 보내는 것이다.
- 시그널을 받았을 때
    
    시그널은 고유의 의미를 내포하고 있다. 이러한 시그널을 받은 실행객체인 프로세스는 그에 맞는 행동을 해야 한다. 시그널을 받은 프로세스는 다음중 한가지 행동을 취해야 한다.
    
    1. 그 시그널을 처리할 **등록된** 함수(handler)를 호출한다.
    2. 시그널을 무시한다.
    3. 시그널을 무시하지 않지만, 그렇다고 해서 특별히 함수를 호출 하지도 않는다.
- 시그널 종료는 다양하고 `signal.h`에 정의 되어있다.

키보드 입력으로 발생시킬 수 있는 시그널은 Ctrl+C 외에도 아래의 몇가지가 있다.

| Ctrl+C | SIGINT | 프로세스를 종료시킨다. |
| --- | --- | --- |
| Ctrl+Z | SIGSTP | 프로세스를 중단시킨다. |
| Ctrl+\ | SIGQUIT | core dump를 남기고 프로세스를 종료시킨다. |
| ctrl+D |  | "end of file"을 의미한다. 터미널이 입력 상태이고, 라인의 맨 처음일 때에만 작동한다. (‘\0’를 STDIN에 입력하는 것) |

**일반적으로 프로세스의 경우 `SIGINT`(Ctrl+C) 시그널을 통하여 수행중인 프로세스(터미널)를 종료시킬 수 있지만 minishell의 경우엔 우리가 만든 minishell만 종료되고 터미널은 여전히 살아있도록 해야한다. 이런식으로 시그널을 받은 프로세스가 취할 행동을 바꿔주는게 handler 함수이다.**

- 하지만 fork()를 통하여 일반 명령을 수행하는 자식 프로세스의 경우엔 작업 도중에 수행을 중단시킬 수 있어야 하므로 자식 프로세스의 경우에 한해서만 `SIGINT`를 DEFAULT로 설정한다.
- 자식 프로세스가 백그라운드로 수행중일 때는 쉘의 뒤편에서 암묵적으로 수행하는 프로세스이므로 `SIGINT` 시그널을 무시하도록 설정한다.

### readline()

`char *readline(const char str*);`

str를 출력하고 프롬프트를 열어서 표준입력으로 문자열을 입력받는다. 개행(엔터)를 받으면 지금까지 입력된 문자열을 리턴한다. 문자열을 입력받기 전까지는 다음 코드로 진행되지 않는다. `rl_replace_line`, `rl_redisplay` 등으로 프롬프트가 비워져도 계속 문자열을 받는 상태가 된다.

### rl_replace_line()

`void rl_replace_line(const char str*, int);`

GNU 라이브러리에만 들어있는 함수이다.

현재까지 입력된 프롬프트의 문자열을 str로 바꿔준다.

ctrl + C 처럼 프롬프트를 입력하지는 않고 새로운 프롬프트를 출력해야 할 때 `rl_replace_line(””, 1);`처리를 해준다면 새로운 프롬프트를 비워줄 수 있게 된다.

### rl_on_new_line()

`int rl_on_new_line(void);`

`rl_redisplay`를 실행하기 위해 필요한 함수이다

### rl_redisplay()

`void rl_redisplay(void);`

rl_replace_line를 출력하지 않으면 작동하지 않는다. `readline` 함수의 인자로 넣은 문자열을 다시 출력한다.

### add_history()

`int add_history(const char *);`

인자에 넣은 문자열을 history로 저장한다.

프롬프트가 열린 상태에서 키보드 방향키 위 아래를 통해 이제껏 프롬프트에 입력한 문자열을 불러올 수 있다. 스택처럼 가장 마지막에 넣은 문자열부터 불러온다.


## 🌱 shell 🌱

### **shell이란?**

셸은 컴퓨터와 상호 작용할 수 있는 응용 프로그램이다. 셸에서 사용자는 프로그램을 실행할 수 있으며, 입력과 출력을 파일에서 가져오도록 리디렉션할 수도 있다. 셸은 또한 함수, 변수 등과 같은 프로그래밍 구조를 제공한다. 셸 스크립트라고 불리는 셸 프로그램은 편집, 기록, 파일 완성, 와일드카드, 환경 변수 확장 및 프로그래밍 구성과 같은 기능을 제공한다.

### **shell의 구성요소**

셀을 구현하는 작업은 네 부분으로 나눔!₩

### Lexer

- 소스코드를 토큰단위로 분석한다.

### Parser

- 명령을 읽은 뒤 *Command Table* (명령 테이블)이라는 데이터 구조에 삽입해 실행될 명령을 저장한다.

### Executor

- 명령 테이블의 모든 명령에 대해 새 프로세스를 생성한다.
    - 필요한 경우 파이프를 생성하여 한 프로세스의 출력을 다음 프로세스의 입력으로 전달한다.
        
        표준 입력, 표준출력, 표준오류를 리디렉션(<, >, >>)한다.
        

### Shell subsystems

- **Environment Variables(환경변수)**
    
    : `${VAR}` 로 환경변수를 불러올 수 있다. 셸은 환경변수를 설정, 확인 및 출력할 수 있어야 한다. 
    
- **Wildcards(와일드카드)**
    
    : `*` 은 문자열 와일드 카드이다. 해당 디렉토리에서 내용이 일치하는 모든 파일을 불러온다.
    
- **Subshells**
    
    : `( )`, `$( )`, `|`, `&` 를 이용해 한 명령의 출력값을 새 명령의 입력값으로 활용할 수 있다. 이렇게 명령을 실행시킬 때 생성되는 shell을 subshell 이라고 한다.
    
    - parent process 에서 설정한 변수나 함수는 export 해야지만 child process 에서 사용할 수 있다. 하지만 subshell 에서는 export 하지 않아도 사용할 수 있는 것이 특징이다.

### shell에서 프로세스 생성하는 방법

Unix에서 프로세스를 시작하는 방법은 두 가지 뿐!

- Init
- fork()
    
    : 대부분의 프로그램은 Init이 아니기 때문에 프로세스를 시작하는 실질적인 방법은 `fork()` syscall 뿐이다. 이 기능이 호출되면 운영 체제가 부모로 프로세스로부터 자식 프로세스를 복제하여 두 프로세스를 병렬로 실행한다.
    
    1. 기존 프로세스는 두 개의 분리된 프로세스로 분기된다.
    2. 그런 다음 자식 프로세스는 `exec()`을 사용하여 자신을 새 프로그램으로 바꾼다.
    3. 부모 프로세스는 다른 작업을 계속 수행할 수 있으며 `wait()`를 사용하여 자식 프로세스를 계속 감시할 수도 있다.



## 🍀 token 🍀
Compiler의 첫 번째 단계는 Lexical Analysis로 가장 먼저 Lexical Analyzer를 실행합니다.

### **Lexical Analyzer 란?**

Lexical Analyzer는 Input 문자열들을 스캔하면서 의미가 있는 단위로 문자열을 묶습니다. (그래서 Scanner라고도 불립니다). 그리고 일련의 Token들을 생성해 내고 Syntax Analyzer에게 보냅니다. 이 과정을 진행하면서 Token들에 대한 정보들을 Symbol Table에 저장합니다.

<img width="523" alt="Screen Shot 2023-05-02 at 1 58 28 AM" src="https://user-images.githubusercontent.com/77817094/235492595-da5511e4-0674-44e4-a39d-442c16eac107.png">


### **Token 이란?**

Token은 token name, token value의 쌍입니다. (token value는 생략해도 됩니다).

#### **> token name은 특정한 어휘를 대표하는 symbol입니다.**

- IDENTIFIER: sum, num, i, j, k,... 같은 (Keyword가 아닌) 일련의 문자들을 대표
- NUMBER: 1, 2, 3, 2.7, ... 같은 숫자들을 대표
- ARITHMETIC OPERATOR: , /, +, -, % 와 같은 산술 연산자들을 대표
- ASSIGN: = 과 같은 대입 연산자를 대표

#### **> token value는 token name의 속성 값입니다.**

- IDENTIFIER: sum, num, i, j, k과 같은 것들
- NUMBER: 1, 2, 3, 2.7과 같은 것들
- ARITHMETIC OPERATOR: , /, +, -, % 와 같은 것들
- ASSIGN: '='인 것이 자명하므로 token value 생략가능

#### **Token 예**

- <IDENTIFIER, sum> <IDENTIFIER, num> <NUMBER, 2> <ARITHMETIC OPERATOR, +> <ASSIGN>
    
    **Token**은 **token name**과 **token value**의 쌍이라고 했습니다.이와 같은 쌍을 표현하기 위해 앞으로 Token은 **<token name, token value>**의 형태로 나타내겠습니다.
    

### **다양한 Token 들 알아보기!**

- Keywords: IF(if), ELSE(else), FLOAT(float), CHAR(char)
- Operators: ADD(+), COMPARISON(<,>,...)
- Identifiers: ID(모든 종류의 식별자)
- Constants: NUMBER(모든 숫자 형태의 상수: 정수, 실수, 리터럴)
- Punctuation Symbols: LBRACE({), COMMA(,)
- Whitespace: 띄어쓰기, 새 줄 문자(\n), 탭들의 비어 있지 않은 열거.※ Lexical Analyzer는 parsing 하는데 필요 없는 것들은 버립니다. (예) whitespace, comment

## ****Lexeme 란?****

Lexeme는 Input Stream(소스 코드)의 문자들 중에서 Token의 Pattern을 만족하는 일련의 문자들 입니다! > 여기서 Pattern은 Token을 정의하기 위한 rule들의 집합이라고 이해하시면 됩니다.

### **Lexeme 예**
 
 <img width="451" alt="Screen Shot 2023-05-02 at 2 00 12 AM" src="https://user-images.githubusercontent.com/77817094/235492859-ecc4d13b-c68c-424f-811b-788640a2dd7d.png">

