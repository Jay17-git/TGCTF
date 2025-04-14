FROM golang:1.21-alpine

WORKDIR /app

COPY main.go .
COPY templates/ ./templates/
COPY static/ ./static/
COPY notes/ ./notes/
COPY flag /flag

RUN go mod init zyll \
    && go build -o main . \
    && chmod 755 notes \
    && adduser -D -u 1000 appuser \
    && chown -R appuser /app

USER appuser

EXPOSE 9046

CMD ["./main"]
