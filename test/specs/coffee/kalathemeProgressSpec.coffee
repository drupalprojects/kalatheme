(($) ->
  describe 'kalathemeProgress bar is a replacement for Drupal core progress bars' , ->
    it 'adds a constructor on Drupal.progressBar', ->
      expect(Drupal.progressBar).toBeDefined()
    describe 'the constructor', ->
      testProgressBar = null

      updateCallback = (e)->
        console.log 'update called', e
      method = (e) ->
        console.log 'method called', e
      errorCallback = (e)->
        conso
      testProgressBar = new Drupal.progressBar 'testProgressBar', updateCallback, method, errorCallback
      window._testObjs ?= {}
      _testObjs.testProgressBar = testProgressBar
      it 'constructs DOM element with matching IDs as a parameter', ->
        expect(testProgressBar).toBeDefined()
        expect(testProgressBar.element).toContainElement('#testProgressBar')
      it 'constructs accessible markup to the page using ARIA Roles', ->
        el = testProgressBar.element
        expect(el).toHaveAttr('aria-live', 'polite')
        expect( $(el).find('#testProgressBar') ).toHaveAttr('aria-describedby', 'messagetestProgressBar')
        expect($(el).find('.progress-bar') ).toHaveAttr('role', 'progressbar')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemin', '0' )
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuemax','100')
        expect($(el).find('.progress-bar') ).toHaveAttr('aria-valuenow','0')
        expect($(el).find('#messagetestProgressBar')).toExist()

)(jQuery)
