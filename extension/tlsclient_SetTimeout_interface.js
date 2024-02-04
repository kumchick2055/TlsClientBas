<div class="container-fluid">
 	<%= _.template($('#input_constructor').html())({
        id:"Timeout", 
        description:tr("Timeout (seconds)"), 
        default_selector: "int", 
        disable_string:true, 
        value_number: 10,
        min_number:1,
        max_number:999999, help: {description: tr("Maximum request execution time.")} 
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Set Maximum request execution time with http client.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>