set(server_top_srcs
    feedback.html
)

set(server_analytics_srcs
    analytics/index.php
    analytics/.htaccess
)

set(server_config_srcs
    config/config.php
    config/.htaccess
)

set(server_shared_srcs
    shared/analytics.php
    shared/datastore.php
    shared/product.php
    shared/receiver.php
    shared/restdispatcher.php
    shared/restexception.php
    shared/schema.json
    shared/schemaentry.php
    shared/schemaentryelement.php
    shared/survey.php
    shared/utils.php
    shared/.htaccess
)

set(server_receiver_srcs
    receiver/index.php
    receiver/.htaccess
)

php_lint(${server_analytics_srcs})
php_lint(${server_config_srcs})
php_lint(${server_shared_srcs})
php_lint(${server_receiver_srcs})

if (SERVER_DEPLOY_DESTINATION)
    add_custom_target(deploy
        COMMAND scp ${server_top_srcs} ${SERVER_DEPLOY_DESTINATION}/
        COMMAND scp ${server_analytics_srcs} ${SERVER_DEPLOY_DESTINATION}/analytics
        COMMAND scp ${server_config_srcs} ${SERVER_DEPLOY_DESTINATION}/config
        COMMAND scp ${server_receiver_srcs} ${SERVER_DEPLOY_DESTINATION}/receiver
        COMMAND scp ${server_shared_srcs} ${SERVER_DEPLOY_DESTINATION}/shared
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
