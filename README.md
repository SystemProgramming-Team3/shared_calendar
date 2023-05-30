## SHARED! Calendar

이 프로그램은 콘솔 환경에서도 편리한 일정 관리를 위해 개발되었습니다. 서버-클라이언트 모델을 채택하여 여러 클라이언트에서 접속하여 일정을 확인 및 수정할 수 있으며, 클라이언트 컴퓨터의 로컬 저장소가 삭제되더라도 데이터가 유실되지 않습니다.

### 화면 구성 및 주요 기능

- 캘린더 표시 화면: 선택한 월의 캘린더를 표시하고, 일정이 있는 날짜에 강조 표시가 되어 있어 쉽게 확인할 수 있습니다.
- 일정 표시 화면: 선택한 날짜의 모든 일정을 한 눈에 확인할 수 있습니다.
- 명령어 화면: 원하는 명령어를 입력하여 실행할 수 있습니다.
  - 일정 추가: 원하는 월, 일에 일정을 추가합니다.
  - 일정 검색: 특정 날짜에 있는 모든 일정을 확인합니다.
  - 일정 날짜 변경: 일정이 변경된 경우 날짜를 변경할 수 있습니다.
  - 일정 삭제: 일정을 삭제할 수 있습니다.

### 시스템 최소 요구 사항

- Debian 계열의 Linux(또는 POSIX 계열의 Linux 또는 UNIX) 및 x86계열의 CPU가 필요합니다.


### 사용 방법

1. Repository Clone

   ```
   git clone https://github.com/your-username/calendar.git
   ```

2. 프로그램을 컴파일합니다. (또는 make 명령어 실행)

   ```
   gcc -o input input.c calClient.c //클라이언트 시스템에서 실행
   gcc -o calServer calServer.c // 서버 시스템에서 실행
   ```
   make를 Shell에서 실행할 경우 Makefile의 Script에 의해 자동으로 컴파일되므로 gcc 명령어를 수동으로 실행하지 않아도 됩니다.

4. 캘린더 프로그램을 실행합니다.

   ```
   ./calServer (포트 번호) // 서버 시스템
   ./input //클라이언트 시스템
   ```

### 캘린더 프로그램 조작 방법

1. 메인 화면
![main](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/4e94886b-de7f-4808-87ec-80e80e23168b)
- 캘린더 프로그램의 메인 화면입니다.
  - CALENDAR 메뉴: 현재 월의 달력을 확인할 수 있습니다.
  - CONTENTS 메뉴: 선택한 날짜의 일정을 확인할 수 있습니다.
  - USAGE EXAMPLE 메뉴: 명령어 입력 방법과 예시를 확인할 수 있습니다.
  - COMMAND 메뉴: 캘린더 프로그램의 명령어를 입력할 수 있습니다.

2. 캘린더에 일정 추가 (a 명령어)
![내용 추가](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/55bafd56-04f7-4dd9-97c3-2405ed6a30d4)
- a 명령어를 이용하여 캘린더에 일정을 추가합니다.
- 입력 예시: a (날짜) (제목)>(내용)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.

3. 특정 일자의 일정을 확인 (C 명령어)
![특정일자일정전](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/f6622dc3-4f73-4e71-a9ae-dd0262113ef3)
[명령어 입력 예시 화면]
![특정일자일정후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/179db79d-6de7-4a3b-8547-75d3ca1dc25a)
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
- c 명령어를 이용하여 캘린더에 저장되어 있는 특정 일자의 일정 전체 목록을 확인할 수 있습니다.
- 입력 예시: c (날짜)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.
