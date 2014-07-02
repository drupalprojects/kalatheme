module.exports =
  theme:
    dir: [
      '{includes,views,styles}/**/*.inc'
      './*.{inc,php}'
    ]
  options:
    bin: 'vendor/squizlabs/php_codesniffer/scripts/phpcs'
    standard: 'vendor/drupal/coder/coder_sniffer/Drupal'
