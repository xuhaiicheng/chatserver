#定义了一个SRC_LIST变量，包含了该目录下所有的源文件
aux_source_directory(. SRC_LIST)
aux_source_directory(./db DB_LIST)
aux_source_directory(./model MODEL_LIST)
aux_source_directory(./redis REDIS_LIST)

#指定生成可执行文件
add_executable(ChatServer ${SRC_LIST} ${DB_LIST} ${MODEL_LIST} ${REDIS_LIST})

#指定可执行文件链接时需要依赖的库文件
target_link_libraries(ChatServer muduo_net muduo_base -L/usr/lib64/mysql -lmysqlclient hiredis pthread)
#-L/usr/lib64/mysql -lmysqlclient 这个是mysqlclient库的绝对路径 意思是 不从默认的文件夹/usr/lib里边取动态库-lmysqlclient从绝对目录里边取。
#或者直接复制一份到/usr/lib一份就行了。