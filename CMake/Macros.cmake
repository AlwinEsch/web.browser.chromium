macro(core_add_library name)
  add_library(${name} STATIC ${SOURCES} ${HEADERS})
  set(CORE_LIBRARY ${name} ${CORE_LIBRARY} PARENT_SCOPE)
endmacro()
