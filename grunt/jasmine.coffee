module.exports =
  specs:
    src: "dist/js/kalatheme.pkg.js"
    options:
      specs: 'test/specs/**/*Spec.js'
      vendor: [
        "bower_components/jquery/dist/jquery.js"
        "bower_components/bootstrap/dist/js/bootstrap.js"
        "bower_components/jasmine-ajax/lib/mock-ajax.js"
        "bower_components/jasmine-jquery/lib/jasmine-jquery.js"
        # this is not a great thing, but I need the tests to be semi independent.
        "temp/drupaljs/jquery.once.js"
        "temp/drupaljs/drupal.js"
        "temp/drupaljs/ajax.js"
        "temp/drupaljs/autocomplete.js"
        ]
      outfile: 'test/specs/_SpecRunner.html'
      keepRunner: true
