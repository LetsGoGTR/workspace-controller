FROM atmoz/sftp:latest


RUN groupadd -g 1000 default \
    && useradd -u 1000 -g default -d /home/default -s /bin/bash -m default

RUN chown -R default:default /home/default


# SFTP 서버 시작 시 default 사용자로 실행하도록 설정
CMD [ "default:1234:1000:1000" ]
