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

- Debian 계열의 Linux(Debian, Ubuntu 등) 및 x86계열의 CPU가 필요합니다.
- UNIX 및 다른 POSIX Linux도 호환되지만, 모든 동작이 정상적으로 실행됨을 보장하지는 않습니다.

### 필요한 구성 요소 설치
- ncurses 라이브러리를 설치해야 정상적으로 프로그램이 동작합니다.
- apt-get을 이용하여 설치해 주세요.

```
sudo apt-get update
sudo apt-get install libncurses5-dev libncursesw5-dev
```

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
<br>
[명령어 입력 예시 화면]
<br>
![cc실행이후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/01b7dbdc-ab63-42de-a974-8ed01b4a2c6b)

<br>
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
<br>
- a 명령어를 이용하여 캘린더에 일정을 추가합니다.
- 입력 예시: a (날짜) (제목)>(내용)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.
- (예: abcde라는 제목과 SystemProgramming이라는 내용의 일정을 2023년 5월 25일에 추가하려면 a 2023.05.25 abcde>SystemProgramming 으로 입력합니다.)

3. 특정 일자의 일정을 확인 (c 명령어)
![특정일자일정전](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/f6622dc3-4f73-4e71-a9ae-dd0262113ef3)
<br>
[명령어 입력 예시 화면]
<br>
![특정일자일정후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/179db79d-6de7-4a3b-8547-75d3ca1dc25a)
<br>
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
<br>
- c 명령어를 이용하여 캘린더에 저장되어 있는 특정 일자의 일정 전체 목록을 확인할 수 있습니다.
- 입력 예시: c (날짜)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.
- (예: 2023년 5월 25일의 일정을 확인하려면 c 2023.05.25 로 입력합니다.)

4. 일정의 날짜를 변경 (mv 명령어)
![날짜변경전](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/709fb79e-66fc-48b5-8e59-b15b82bddde7)
<br>
[명령어 입력 예시 화면]
<br>
![날짜변경후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/d729bc56-4cda-4032-b60d-9555fe51c30e)
<br>
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
<br>
- mv 명령어를 이용하여 캘린더에 저장되어 있는 일정의 날짜를 변경할 수 있습니다.
- 입력 예시: mv (변경 전 날짜) (변경 후 날짜) (일정의 제목)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.
- (예: abcde라는 제목의 일정을 2023년 5월 25일에서 2023년 5월 30일로 변경하려면 mv 2023.05.25 2023.05.30 abcde 로 입력합니다.)

5. 해당 월의 전체 일정 확인 (cc 명령어)
![cc명령어전](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/f5488464-47cb-4523-a72a-374cbd26ffa9)
<br>
[명령어 입력 예시 화면]
<br>
![cc명령어후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/a0440d0d-7b9e-4331-9a4d-5164705f3597)
<br>
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
<br>
- cc 명령어를 이용하여 선택한 월의 일정이 존재하는 모든 날짜를 확인할 수 있습니다.
- 입력 예시: cc (확인할 월)
- 확인할 월은 YYYY.MM (연.월) 순서로 입력합니다.
- (예: 2023년 5월을 확인하려면 cc 2023.05 로 입력합니다)

6. 일정 삭제 (rm 명령어)
![rm명령어전](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/46559d89-8aec-4c81-bd64-04a72a5366e1)
<br>
[명령어 입력 예시 화면]
<br>
![rm명령어후](https://github.com/SystemProgramming-Team3/shared_calendar/assets/62194473/34147fb6-3fab-423c-bbad-49318b5c0f50)
<br>
[명령어 입력 후 Enter 키를 누르고 명령이 실행된 화면]
<br>
- rm 명령어를 이용하여 특정 일정을 삭제할 수 있습니다.
- 입력 예시: rm (삭제할 일정이 속한 날짜) (삭제할 일정의 제목)
- 날짜의 입력 예시는 YYYY.MM.DD (연.월.일) 입니다.
- (예: 2023년 5월 25일의 abcde라는 제목의 일정을 삭제하려면 rm 2023.05.25 abcde 로 입력합니다.)
