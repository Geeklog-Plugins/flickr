/* 
* Mini Flickr gallery jQuery plugin
* For geeklog 1.6.0+
* Authors: Felipe Skroski | ::Ben
*/

function initialize_flickr(){
	if($('.flickr-mini-gallery')){
		$('.flickr-mini-gallery').each(function (i) {
				$(this).empty();					 
				var filter = $(this).attr('rel');
				//build_gallery(filter, this);
		});
	} else if($('.flickr-image')){
	    $('.flickr-image').each(function (i) {
		$(this).empty();					 
		var filter = $(this).attr('rel');
		build_image(filter, this);
		});
	}
	else{
	}
}
function build_gallery(filter, obj){
  var api = "https://api.flickr.com/services/rest/?method=flickr.photos.search&api_key="+flickr_api_key+"&tag_mode=all&"+filter; 
	$.getJSON(api+"&format=json&jsoncallback=?",
        function(data){
          $.each(data.photos.photo, function(i,item){
		  $("<img/>").attr({src: "http://static.flickr.com/"+item.server+"/"+item.id+"_"+item.secret+flickr_mini_gallery_img_format+".jpg", alt:item.title}).appendTo(obj).wrap("<a href=http://static.flickr.com/"+item.server+"/"+item.id+"_"+item.secret+".jpg alt="+item.id+"></a>");
         });
		 $(".flickr-mini-gallery a:has(img)").lightBox();
      });	 
}
function build_image(filter, obj){
  var api = "https://api.flickr.com/services/rest/?method=flickr.photos.search&api_key="+flickr_api_key+"&tag_mode=all&"+filter; 
	$.getJSON(api+"&format=json&jsoncallback=?",
        function(data){
          $.each(data.photos.photo, function(i,item){
		  $("<img/>").attr({src: "http://static.flickr.com/"+item.server+"/"+item.id+"_"+item.secret+flickr_mini_gallery_img_format+".jpg", alt:item.title}).appendTo(obj).wrap("<a href=http://static.flickr.com/"+item.server+"/"+item.id+"_"+item.secret+".jpg alt="+item.id+"></a>");
         });
		 $(".flickr-image a:has(img)").lightBox();
      });	 
}
function add_description(n){
	var img_id = '2388852124';
	$.getJSON('https://api.flickr.com/services/rest/?method=flickr.photos.getInfo&api_key='+flickr_api_key+'&photo_id='+img_id+'&format=json&jsoncallback=?',
        		function(data){
					var textInfo = data.photo.description._content;
					$(".felickr:first").append(textInfo+"<br/>");					
         	});
}
function description(){
	$(".felickr img").each(function(i){
			add_description(i)						
	})
}

//jQuery(document).ready(function(){
//    initialize_flickr()	;
//});