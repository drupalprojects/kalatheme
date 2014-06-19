module.exports = ->
  $ = require('jquery')
  $(->
    toggle = $('.dropdown-toggle')
    toggle.attr({
      'aria-expanded': 'false'
    })
    $('.dropdown').on('shown.bs.dropdown', (e) ->
      $(@).find('.dropdown-toggle').attr({'aria-expanded': 'true'})
    ).on('hidden.bs.dropdown', (e) ->
      $(@).find('.dropdown-toggle').attr({'aria-expanded': 'false'})
    )
  )
