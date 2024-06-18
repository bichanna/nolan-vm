project('NolanVM', 'cpp',
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true'
  ]
)

include=include_directories('include')

src=[
  'src/main.cpp'
]

executable('nolanvm',
  src,
  include_directories: include,
  install: true
)
