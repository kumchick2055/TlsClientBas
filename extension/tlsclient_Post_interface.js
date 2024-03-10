<div class="container-fluid">
	<%= _.template($('#input_constructor').html())({
        id:"Value", 
        description:tr("Url"), 
        default_selector: "string", 
        disable_int:true, 
        help: 
        {
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
	
	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<hr style="margin-top:5px;margin-bottom:5px"/>
				<span data-preserve="true" data-preserve-type="check" data-preserve-id="Check2">
					<input type="checkbox" id="Check2" onclick="$('#WithPostData').toggle();$('#NoPostData').toggle()"/> <label for="Check2" class="tr">Post data constructor</label>
				</span>
				<hr style="margin-top:5px;margin-bottom:5px"/>
			</div>
		</form>
	</div>

	<span id="WithPostData" style="display:none">
		<div class="col-xs-12">
			<form class="form-horizontal">
				<div class="form-group">
					<%= _.template($('#input_constructor').html())({
                        id:"PostName", 
                        description:tr("Request Parameter Name"), 
                        default_selector: "string", 
                        disable_int:true, 
                        dont_use_top: true, 
                        help: {
                            description: tr("Post parameter name. After setting this value you need to add it to 'Post Data' parameter by hitting '+ Add' button.")}
                    }) %>
				</div>
			</form>
		</div>

		<div class="col-xs-12">
			<form class="form-horizontal">
				<div class="form-group">
					<%= _.template($('#input_constructor').html())({
                        id:"PostValue", 
                        description:tr("Request Parameter Value"), 
                        default_selector: "string", 
                        disable_int:true, 
                        dont_use_top: true, 
                        help: {description: tr("Post parameter value. After setting this value you need to add it to 'Post Data' parameter by hitting '+ Add' button.")} 
                    }) %>
				</div>
			</form>
		</div>


		<div class="col-xs-12">
			<form class="form-horizontal">
				<div class="form-group">
					<button tabindex="-1" type="button" class="btn btn-default addtopost" > + <span class="tr">Add</span></button>
					<button tabindex="-1" type="button" class="btn btn-default addtopostfile" > + <span class="tr">Add As File</span></button>
					<button tabindex="-1" type="button" class="btn btn-default addtopostbase64" > + <span class="tr">Add As Base64 Data</span></button>
				</div>
			</form>
		</div>

		<%= _.template($('#input_constructor').html())({
            id:"PostDataArray", 
            description:tr("Post Data"), 
            default_selector: "expression", 
            disable_int:true, 
            disable_string:true, 
            help: {description: tr("To add values into post data list, fill 'Request Parameter Name' and 'Request Parameter Value' and hit '+ Add' button. This list of key value parameters will be sended to server in form, which is defined in 'Content Type' parameter.")}
        }) %>

		<%= _.template($('#input_constructor').html())({
            id:"ContentType", 
            description:tr("Content Type"), 
            default_selector: "string", 
            disable_int:true, 
            value_string: "urlencode", 
            variants:["urlencode","multipart","json"], 
            size:3, 
            help: {description: tr("Content type with 3 options available: urlencode, multipart, json"), examples:[{code:"urlencode",description:tr("Send data as key/value pairs.")},{code:"multipart",description:tr("Send data as key/value pairs. Values may have binary format, can be used to send files.")},{code:"json",description:tr("Send data in json format.")}]}
        }) %>      
	</span>

	<span id="NoPostData">
		<%= _.template($('#input_constructor').html())({
            id:"PostDataRaw", 
            description:tr("Post Data"), 
            default_selector: "string", 
            disable_int:true, 
            disable_editor:true, 
            disable_expression:true, 
            use_textarea:true, 
            replace_linebreaks: true, 
            size: 8, 
            disable_type_chooser:true,
            textarea_height:80, 
            modal_code_id:"post-data-" + (_K || "en")
        }) %>
        
		<%= _.template($('#input_constructor').html())({
            id:"ContentTypeRaw", 
            description:tr("Content-Type"), 
            default_selector: "string", 
            disable_int:true, 
            value_string: "application/x-www-form-urlencoded", 
            variants:["application/x-www-form-urlencoded","multipart/form-data","application/json"],
            size:3,
            help: {description: tr("'Contect-Type' header. This parameter indicates what type of content is located in 'Post Data' field."), examples:[{code:"application/x-www-form-urlencoded",description:tr("Data contains key/value pairs separated by &(ampersand). For example, key1=value1&key2=value2&key3=value3")},{code:"multipart/form-data",description:tr("Data contains key/value pairs, values may have binary format, can be used to send files. Boundary will be automatically detected from 'Post Data' field.")},{code:"application/json",description:tr("Send data in json format. For example, {\"key1\":\"value1\",\"key2\":\"value2\",\"key3\":\"value3\"}")}]}
        }) %>      
	</span>

	<div class="col-xs-12">
		<form class="form-horizontal">
			<div class="form-group">
				<hr style="margin-top:5px;margin-bottom:5px"/>
				<span class="tr" style="margin-left:15px;">Additional Parameters</span>:
				<hr style="margin-top:5px;margin-bottom:5px"/>
			</div>
		</form>
	</div>

	<%= _.template($('#input_constructor').html())({
        id:"Method", 
        description:tr("Method"), 
        default_selector: "string", 
        disable_int:true, 
        value_string: "POST", 
        variants:["POST","PUT","PATCH"], 
        size:3, 
        help: {description: tr("Request method, you can put any string here.")}
    }) %>
    
    <%= _.template($('#input_constructor').html())({
        id:"MaximumFailes",
        description:tr("Maximum failures"), 
        default_selector: "int",
        disable_string:true, 
        value_number: 3,
        min_number:1,
        max_number:999999, 
        help: {
            description: tr("The maximum number of failed requests after which the action will be completed with an error.")
        } 
    }) %>
    
<div data-preserve="true" data-preserve-type="check" data-preserve-id="Check">
		<input type="checkbox" id="Check" checked="checked"/> <label for="Check" class="tr">Follow redirects</label>
	</div>
</div>

<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Make http request using POST, PUT or PATCH method,  
i.e. methods which send data.</div>
	<div class="tr tooltip-paragraph-fold">Post data can be specified in 2 forms: raw(default) and with constructor. It is recommended to use raw form.</div>
	<div class="tr tooltip-paragraph-fold">With raw form you need to input post data as is. This means, that you can get post data from sniffer and put it inside 'Post data' field. Check help for this field, it contains a lot of examples. Another thing, which need to be done is setting up 'Content-Type' parameter, it can be obained as one of request headers. 'Content-Type' parameter indicates what type of content is located in 'Post Data' field, it could be json, form content, etc.</div>
	<div class="tr tooltip-paragraph-fold">Variables and resources may be embedded into 'Post data' field. If you need to put binary data, read it into variable with base64 encoding(for example by reading file in base64) and instead of using [[VARIABLE]] notation, use it with 'base64:' prefix, like this base64:[[VARIABLE]]. 'Post data' help contains more examples.</div>
	<div class="tooltip-paragraph-fold"><span class="tr">If 'Record HTTP request' mode is enabled, then every request that browser does will be converted to actions using http client and added to scenario tab</span> ( <a href="#" class="tr" onclick="BrowserAutomationStudio_OpenUrl('https://wiki.bablosoft.com/lib/exe/fetch.php?cache=&media=recordhttprequests.png');return false">screen</a> ). <span class="tr">It is the easiest way to generate action with http requests.</span></div>
	<div class="tr tooltip-paragraph-fold">Special characters and non-latin query parts must be encoded with encodeURIComponent function. For example, instead of using https://www.google.com/search?q=коты, url must be https://www.google.com/search?q=%D0%BA%D0%BE%D1%82%D1%8B</div>

	<div class="tr tooltip-paragraph-fold">HTTP headers(like User-Agent) can be specified in this action with 'Headers' parameter or with 'HTTP-Client Set Header' action. In first case headers will be modified only for current action, in second case headers will be modified for all following actions.</div>
	<div class="tr tooltip-paragraph-fold">By default http client will follow redirects. If you want, you can disable that behavior and process Location header yourself. To get responce header, use 'Get Header' action after request.</div>
	<div class="tr tooltip-paragraph-fold">'Post' action will not set any variable, to obtain responce you need to call 'Current Url', 'Content', 'Status' or 'Get Header' after request is performed.</div>
	<div class="tr tooltip-paragraph-fold">'Current Url' will get actual url after all redirects.</div>
	<div class="tr tooltip-paragraph-fold">'Content' will return page html.</div>
	<div class="tr tooltip-paragraph-fold">'Status' returns HTTP status, for example, 200 - if request is performed correctly, 500 - in case of server error, etc.</div>
	<div class="tr tooltip-paragraph-fold">'Get Header' returns responce header, like 'Content-Type', 'Date', 'Set-Cookie'</div>
	<div class="tr tooltip-paragraph-fold">HTTP client handles cookies automatically, so normally you don't need to process 'Set-Cookie' header.</div>
	<div class="tr tooltip-paragraph-fold">The fastest way to extract data from page source is xpath. 'Http Client' has set of actions to perform xpath queries. It is not recomended to use 'Content' action and regular expressions unless it is absolutely necessary.</div>
	<div class="tr tooltip-paragraph-fold">If server returns json data, use JSON.parse function to parse it.</div>

	<div class="tr tooltip-paragraph-last-fold">You can set max page loading time by clicking on hourglass icon near cancel button.</div>
</div>
<%= _.template($('#back').html())({action:"executeandadd", visible:true, wiki: "how_to_make_post_request"}) %>