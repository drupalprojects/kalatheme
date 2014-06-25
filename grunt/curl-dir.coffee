module.exports = () ->
  baseurl = 'https://raw.githubusercontent.com/drupalprojects/drupal/7.x'
  drupaljs:
    src: [
      baseurl + '/misc/drupal.js'
      baseurl + '/misc/jquery.once.js'
      baseurl + '/misc/ajax.js'
      baseurl + '/misc/autocomplete.js'
      ]
    dest: './temp/drupaljs'
  fontawesomeicons:
    src:
      'https://raw.githubusercontent.com/FortAwesome/Font-Awesome/master/src/icons.yml'
    dest: './temp/icons.yml'
