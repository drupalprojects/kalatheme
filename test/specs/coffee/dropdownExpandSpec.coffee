module.exports = ($) ->
  dropdownExpand = require('./../../coffee/dropdownExpand.coffee')
  toggle = ''
  menu = ''
  container = ''

  describe('dropdownExpand module', ->
    beforeEach(->
      toggle = $('<button class="dropdown-toggle" data-toggle="dropdown" />')
      menu = $('<ul class="dropdown-menu"></ul>')
      container = sandbox({class: 'dropdown'})
      container
        .append(toggle)
        .append(menu)
      appendSetFixtures(container)
      )
    it('adds aria attributes', ->

      expect(toggle).not.toHaveAttr('aria-haspopup','false')
      expect(toggle).not.toHaveAttr('aria-expanded','')
      dropdownExpand()
      expect(toggle).toHaveAttr('aria-haspopup','true')
      expect(toggle).toHaveAttr('aria-expanded','false')
    )
    it('listens for the shown event', ->
      dropdownExpand()
      shown = spyOnEvent('#sandbox','shown.bs.dropdown')
      hidden = spyOnEvent('#sandbox','hidden.bs.dropdown')
      expect(toggle).toHaveAttr('aria-expanded','false')
      toggle.trigger('click')
      expect($('.dropdown')).toHaveClass('open')
      expect(toggle).toHaveAttr('aria-expanded','true')
      expect(shown).toHaveBeenTriggered()
    )
  )
