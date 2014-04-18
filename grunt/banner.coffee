module.exports = ->
  return "/*! <%= pkg.title || pkg.name %> - v<%= pkg.version %> - " + "<%= grunt.template.today(\"yyyy-mm-dd\") %>\n" + "<%= pkg.homepage ? \"* \" + pkg.homepage + \"\\n\" : \"\" %>" + "* Copyright (c) <%= grunt.template.today(\"yyyy\") %> <%= pkg.author.name %>;" + " Licensed <%= _.pluck(pkg.licenses, \"type\").join(\", \") %> */\n" + "/*!\n" + "* Bootstrap v3.1.1 (http://getbootstrap.com)\n" + "* Copyright 2011-2014 Twitter, Inc.\n" + "* Licensed under MIT (https://github.com/twbs/bootstrap/blob/master/LICENSE)\n" + "*/\n"

