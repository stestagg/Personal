function load_img(){
	var elm = $("#render");
	var width = elm.innerWidth();
	var height = elm.innerHeight();
	var ratio = (height / width);

	elm.attr("src", "img/?w=" + width + "&h=" + height);
	elm.click(function(e){
		var ox = ((e.offsetX/width) - 0.5);
        var oy = ((e.offsetY/height) - 0.5) * ratio;

        window.location = "scale/" + ox + "/" + oy + "/0.2/";
	});
};

$(function(){

	load_img();
	$("#update_button").click(function(){
		var new_its = $("#iters").val();
		window.location = "../" + new_its + "/";
	});

});