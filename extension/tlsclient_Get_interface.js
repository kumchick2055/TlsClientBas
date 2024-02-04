<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Url"), 
        default_selector: "string", 
        disable_int:true, 
        help: {
            description: tr("Request url"),
            examples:[
            {code:"https://google.com",description:tr("Url with scheme")},
            {code:"google.com",description:tr("Url without scheme")},
            {code:"https://www.google.com/search?q=cats",description:tr("Url with query string")},
            {code:"https://www.google.com/search?q=%D0%BA%D0%BE%D1%82%D1%8B",description:tr("Url with encoded query. To encode string use encodeURIComponent function.")}
           ]
       }
    }) %>
	<%= _.template($('#input_constructor').html())({
        id:"Headers", 
        description:tr("Headers, for example:\nUser-Agent: Mozilla/5.0 Gecko/20100101 Firefox/57.0\nReferer: google.com"), 
        default_selector: "string", 
        disable_int:true, 
        disable_editor:true, 
        disable_expression:true, 
        use_textarea:true, 
        replace_linebreaks: true, 
        size: 8, 
        disable_type_chooser:true,
        textarea_height:80
    }) %>
    
	<%= _.template($('#input_constructor').html())({
        id:"Method", 
        description:tr("Method"), 
        default_selector: "string", 
        disable_int:true, 
        value_string: "GET", 
        variants:["GET","HEAD","DELETE","TRACE","OPTIONS"], 
        size:3,
        help: {description: tr("Request method, you can put any string here.")}
    }) %>
    
    <div data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
        <input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Follow redirects</label>
	</div>
</div>


<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki:"how_to_make_get_request"}) %>