project (luabind)
include_directories( ../boost_1_42_0/ . ../lua5.1/src)
link_directories( ../lua5.1/lib/Linux26g4_64 ../boost_1_42_0/)
add_library(luabind src/class.cpp              src/link_compatibility.cpp
src/class_info.cpp         src/object_rep.cpp
src/class_registry.cpp     src/open.cpp
src/class_rep.cpp          src/pcall.cpp
src/create_class.cpp       src/scope.cpp
src/error.cpp              src/stack_content_by_name.cpp
src/exception_handler.cpp  src/weak_ref.cpp
src/function.cpp           src/wrapper_base.cpp
src/inheritance.cpp
)
target_link_libraries( luabind lua5.1)
SET_TARGET_PROPERTIES(luabind PROPERTIES COMPILE_FLAGS -fPIC )