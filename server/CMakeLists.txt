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
    shared/receiver.php
    shared/restdispatcher.php
    shared/schema.json
    shared/utils.php
    shared/.htaccess
)

set(server_receiver_srcs
    receiver/index.php
    receiver/.htaccess
)

if (SERVER_DEPLOY_DESTINATION)
    add_custom_target(deploy
        COMMAND scp ${server_analytics_srcs} ${SERVER_DEPLOY_DESTINATION}/analytics
        COMMAND scp ${server_config_srcs} ${SERVER_DEPLOY_DESTINATION}/config
        COMMAND scp ${server_receiver_srcs} ${SERVER_DEPLOY_DESTINATION}/receiver
        COMMAND scp ${server_shared_srcs} ${SERVER_DEPLOY_DESTINATION}/shared
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()
