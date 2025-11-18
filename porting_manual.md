### 1. 프로그램 복사
workspace-controller 바이너리 파일을 제어기에 복사합니다.

### 2. 데몬 작동을 위한 서비스 파일 작성
제어기 환경에 맞춰 데몬으로 서버사이드 프로그램을 구동하기 위해 서비스 파일에 대한 수정이 필요합니다.
```
[Unit]
Description=Workspace Controller Service
After=network.target

[Service]
User=
WorkingDirectory=
ExecStart=
Restart=always
StandardOutput=file:/var/log/workspace-controller.log
StandardError=file:/var/log/workspace-controller.err

[Install]
WantedBy=multi-user.target
```

1. User :
    유저 이름을 입력합니다. sftp 서버에서 사용할 유저와 동일하게 설정해주시면 됩니다.
    (동일하게 설정하지 않을경우 압축/압축해제 후 파일 소유자가 달라져 sftp 파일 접근이 불가능할 수 있습니다.)
    ex) test
2. WorkingDirectory : 
    workspace-controller 바이너리 파일이 들어있는 폴더의 절대 경로를 넣어줍니다.
    ex) /home/test/daemon/
3. ExecStart : 
    workspace-controller 바이너리 파일의 절대 경로를 넣어줍니다.
    경로 뒤에 1 ~ 65535 범위의 정수를 실행할 포트로 지정할 수 있습니다.
    ex) /home/test/daemon/workspace-controller 9999

### 3. 서비스 파일 이동
서비스 파일을 systemd 폴더 내부로 이동시킵니다.

ex) /etc/systemd/system/workspace-controller.service

### 4. 서비스 파일 리로드 및 데몬 실행

```bash
sudo systemctl daemon-reload
sudo systemctl enable workspace-controller.service
sudo systemctl start workspace-controller.service
```

### 5. 포트 개방
8888 포트에서 listening 중이니 8888 포트를 외부 개방해주시면 됩니다.