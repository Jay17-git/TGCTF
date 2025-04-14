FROM php:7.3-fpm-alpine3.11
ADD repositories /etc/apk/repositories
ADD default.conf /
ADD index.php /
ADD upload.php /
ADD index.php.bak /
ADD upload.php.bak /
ADD run.sh /
RUN mkdir -p /var/www/html/uploads &&\
    chown -R www-data:www-data /var/www/html/uploads && \
    chmod 755 /var/www/html/uploads
RUN apk update && apk add nginx && \
    apk add m4 autoconf make gcc g++ linux-headers && \
    docker-php-ext-install pdo_mysql opcache mysqli && \
    mkdir /run/nginx && \
    mv /default.conf /etc/nginx/conf.d && \
    mv /index.php /var/www/html && \
    mv /upload.php /var/www/html &&\
    mv /index.php.bak /var/www/html && \
    mv /upload.php.bak /var/www/html &&\
    touch /run/nginx/nginx.pid && \
    chmod 755 /run.sh && \
    chmod 755 /var/www/html/ &&\
    apk del m4 autoconf make gcc g++ linux-headers

EXPOSE 80

ENTRYPOINT ["/run.sh"]
