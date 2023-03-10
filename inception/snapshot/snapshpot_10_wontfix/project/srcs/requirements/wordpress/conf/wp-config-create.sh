#!bin/sh
if [ ! -f "/var/www/wp-config.php" ]; then
cat << EOF > /var/www/wp-config.php
<?php
define( 'DB_NAME', '${DB_NAME}' );
define( 'DB_USER', '${DB_USER}' );
define( 'DB_PASSWORD', '${DB_PASS}' );
define( 'DB_HOST', 'mariadb' );
define( 'DB_CHARSET', 'utf8' );
define( 'DB_COLLATE', '' );
define('FS_METHOD','direct');
\$table_prefix = 'wp_';
define( 'WP_DEBUG', false );
if ( ! defined( 'ABSPATH' ) ) {
define( 'ABSPATH', __DIR__ . '/' );}
define( 'WP_REDIS_HOST', 'redis' );
define( 'WP_REDIS_PORT', 6379 );
define( 'WP_REDIS_TIMEOUT', 1 );
define( 'WP_REDIS_READ_TIMEOUT', 1 );
define( 'WP_REDIS_DATABASE', 0 );
require_once ABSPATH . 'wp-settings.php';
EOF

echo "Generating configuration file for Wordpress..."
wp config create --dbname=$DB_NAME --dbuser=$DB_USER \
    --dbpass=$DB_PASS --dbhost=mariadb
echo "Wordpress configuration file generated!"

echo "Installing Wordpress..."
wp core install --url=localhost --title="Inception" --admin_user=$WP_ADMIN_LOGIN \
    --admin_password=$WP_ADMIN_PASSWORD --admin_email=$WP_ADMIN_EMAIL --skip-email
echo "Wordpress is installed!"

echo "Creating second user.."
wp user create $WP_USERX_LOGIN $WP_USERX_EMAIL --user_pass=$WP_USERX_PASSWORD --role=author
echo "Second user created!"

echo "Installing Wordpress theme..."
wp theme install twentysixteen --activate
echo "Wordpress theme installed!"

fi

echo "Starting PHP-FPM..."
exec php-fpm7 -F -R
