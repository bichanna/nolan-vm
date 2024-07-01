project('NolanVM', 'c',
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c17'
  ]
)

include=include_directories('include')

src=[
  'src/main.c',
]

executable('nolanvm',
  src,
  include_directories: include,
  install: true
)
