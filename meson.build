project('NolanVM', 'cpp',
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true',
    'cpp_std=c++17'
  ]
)

add_project_arguments('-Wno-c++11-extensions', language: 'cpp')

include=include_directories('include')

src=[
  'src/main.cpp',
  'src/gc.cpp',
  'src/util.cpp',
  'src/value.cpp',
  'src/vm.cpp'
]

executable('nolanvm',
  src,
  include_directories: include,
  install: true
)
