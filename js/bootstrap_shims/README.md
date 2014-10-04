JQuery Shims
---------------------

Currently the most current and Kalatheme compatible jQuery version is 1.7 (panopoly, ctools, IPE, etc.)

Bootstrap 3.2 (and some previous versions) set the jQuery dependency to 1.9 (which is hardly bleeding edge).

These shims patch up certain bootstrap classes that dont seem to like jQuery 1.7, this is mostly due to the way in which callbacks are declared and copious use of 
```
var complete = function(){ … }
$.one( 'bs.eventString', complete )
```

generally speaking setting an "onMethodComplete" callback to $.fn.bootstrapComponent.Constructor, and replacing the above with:

```
$.on('bs.eventString', this.onMethodComplete.bind( this ));
```

solves these issues. 

We need to run the bootstrap jUnit tests with these overrides... but its a stable OOP solution. 
