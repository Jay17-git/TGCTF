FROM ctftraining/base_image_nginx_php_73
RUN rm -r /var/www/html/index.php

COPY index.php /var/www/html/
COPY start.sh /start.sh
RUN chmod +x /start.sh
CMD ["/start.sh"]
