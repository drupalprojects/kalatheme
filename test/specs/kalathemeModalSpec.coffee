module.exports = ->
  $ = require('jquery')
  require('../../src/kalathemeModal.coffee')
  describe 'kalathemeModal', ->
    Drupal.CTools.Modal.modalContent = () ->
      return true
    it 'attaches to the Drupal.CTools.Modal object', ->
      expect(Drupal.CTools.Modal).toBeDefined()
    # it 'shows the modal if it isn\'t defined yet', ->
    #   Drupal.CTools.Modal.modalContent = () ->
    #     return true
    #   spyOn(Drupal.CTools.Modal.modalContent)
    #   console.log( Drupal.CTools.Modal.show('foobar') )
