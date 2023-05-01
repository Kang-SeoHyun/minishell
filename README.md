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

 
 
 
 
 
 
 
