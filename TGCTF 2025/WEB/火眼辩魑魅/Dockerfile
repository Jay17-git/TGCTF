FROM php:7.3-fpm-alpine3.11
COPY src/ /var/www/html
ADD repositories /etc/apk/repositories
ADD default.conf /
ADD tgfffffllllaagggggg /
ADD run.sh /
RUN apk update && apk add nginx && \
    apk add m4 autoconf make gcc g++ linux-headers && \
    docker-php-ext-install pdo_mysql opcache mysqli && \
    mkdir /run/nginx && \
    mv /default.conf /etc/nginx/conf.d && \
    touch /run/nginx/nginx.pid && \
    chmod 755 /tgfffffllllaagggggg &&\
    chmod 755 /run.sh && \
    chmod 755 /var/www/html/ &&\
    apk del m4 autoconf make gcc g++ linux-headers

EXPOSE 80

ENTRYPOINT ["/run.sh"]
