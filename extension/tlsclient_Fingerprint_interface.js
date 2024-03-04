<div class="container-fluid">
    <%= _.template($("#combobox").html())({ id: "fingerprint", title: tr("Fingerprint"), search: true, items: [
        {value: "chrome_103", name: "Chrome 103", title: "", selected: true},
        {value: "chrome_104", name: "Chrome 104", title: ""},
        {value: "chrome_105", name: "Chrome 105", title: ""},
        {value: "chrome_106", name: "Chrome 106", title: ""},
        {value: "chrome_107", name: "Chrome 107", title: ""},
        {value: "chrome_108", name: "Chrome 108", title: ""},
        {value: "chrome_109", name: "Chrome 109", title: ""},
        {value: "chrome_110", name: "Chrome 110", title: ""},
        {value: "chrome_111", name: "Chrome 111", title: ""},
        {value: "chrome_112", name: "Chrome 112", title: ""},
        {value: "chrome_116_PSK", name: "Chrome 116 PSK", title: ""},
        {value: "chrome_116_PSK_PQ", name: "Chrome 116 PSK PQ", title: ""},
        {value: "chrome_117", name: "Chrome 117", title: ""},
        {value: "chrome_120", name: "Chrome 120", title: ""},
        
        {value: "safari_15_6_1", name: "Safari 15.6.1", title: ""},
        {value: "safari_16_0", name: "Safari 16.0", title: ""},
        {value: "safari_ipad_15_6", name: "Safari Ipad 15.6", title: ""},
        {value: "safari_ios_15_5", name: "Safari IOS 15.5", title: ""},
        {value: "safari_ios_15_6", name: "Safari IOS 15.6", title: ""},
        {value: "safari_ios_16_0", name: "Safari IOS 16.0", title: ""},
        
        {value: "firefox_102", name: "Firefox 102", title: ""},
        {value: "firefox_104", name: "Firefox 104", title: ""},
        {value: "firefox_105", name: "Firefox 105", title: ""},
        {value: "firefox_106", name: "Firefox 106", title: ""},
        {value: "firefox_108", name: "Firefox 108", title: ""},
        {value: "firefox_110", name: "Firefox 110", title: ""},
        {value: "firefox_117", name: "Firefox 117", title: ""},
        {value: "firefox_120", name: "Firefox 120", title: ""},
        
        {value: "opera_89", name: "Opera 89", title: ""},
        {value: "opera_90", name: "Opera 90", title: ""},
        {value: "opera_91", name: "Opera 91", title: ""},
        
        {value: "zalando_android_mobile", name: "Zalando Android Mobile", title: ""},
        {value: "zalando_ios_mobile", name: "Zalando IOS Mobile", title: ""},
        {value: "nike_ios_mobile", name: "Nike IOS Mobile", title: ""},
        {value: "nike_android_mobile", name: "Nike Android Mobile", title: ""},
        {value: "cloudscraper", name: "CloudScraper", title: ""},
        {value: "mms_ios", name: "MMS IOS", title: ""},
        {value: "mms_ios_1", name: "MMS IOS1", title: ""},
        {value: "mms_ios_2", name: "MMS IOS2", title: ""},
        {value: "mms_ios_3", name: "MMS IOS3", title: ""},
        {value: "mesh_ios", name: "Mesh IOS", title: ""},
        {value: "mesh_ios_1", name: "Mesh IOS1", title: ""},
        {value: "mesh_ios_2", name: "Mesh IOS2", title: ""},
        {value: "mesh_android", name: "Mesh Android", title: ""},
        {value: "mesh_android_1", name: "Mesh Android1", title: ""},
        {value: "mesh_android_2", name: "Mesh Android2", title: ""},
        {value: "confirmed_ios", name: "Confirmed IOS", title: ""},
        {value: "confirmed_android", name: "Confirmed Android", title: ""},
        
        {value: "okhttp4_android_7", name: "OkHTTP4 Android7", title: ""},
        {value: "okhttp4_android_8", name: "OkHTTP4 Android9", title: ""},
        {value: "okhttp4_android_9", name: "OkHTTP4 Android9", title: ""},
        {value: "okhttp4_android_10", name: "OkHTTP4 Android10", title: ""},
        {value: "okhttp4_android_11", name: "OkHTTP4 Android11", title: ""},
        {value: "okhttp4_android_12", name: "OkHTTP4 Android12", title: ""},
        {value: "okhttp4_android_13", name: "OkHTTP4 Android13", title: ""}
        
    ]    
    }) %>
</div>
<div class="tooltipinternal">
	<div class="tr tooltip-paragraph-first-fold">Выберите Браузер отпечаток которого вы хотите установить. Необходимо устанавливать перед выполнением первого запроса, иначе отпечаток не установится. Для решение этого нужно сбросить настройки Http Клиента.</div>
</div>

<%= _.template($('#back').html())({action:"executeandadd", visible:true}) %>