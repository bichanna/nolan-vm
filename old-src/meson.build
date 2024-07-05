project('NolanVM', 'c',
  version: '0.1.0',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'werror=true',
    'c_std=c17'
  ]
)

src=[
  'src/main.c',
  'src/darr.c',
  'src/gc.c',
  'src/le.c',
]

executable('nolanvm',
  src,
  install: true
)
