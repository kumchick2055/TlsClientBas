<div class="container-fluid">
<%= _.template($('#variable_constructor').html())({
    id:"Save", 
    description:tr("Variable To Save"), 
    default_variable: "SAVED_ALL_HEADERS", 
    help: {description: tr("A variable name, which will hold header value.")}
    }) %> 
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Get all response headers</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>