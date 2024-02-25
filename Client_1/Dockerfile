FROM gcc:latest

RUN apt-get update && \
    apt-get install -y cmake libprotobuf-c-dev protobuf-c-compiler

ENV NATS_IP your_ip

COPY . /app

WORKDIR /app

RUN cmake . && make

CMD ["./main"]
