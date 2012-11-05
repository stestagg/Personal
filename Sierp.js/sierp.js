function Point(x, y){
	var self = this;
	self.x = parseInt(x);
	self.y = parseInt(y);
};

Point.prototype.toIndex = function(w){ 
	return parseInt(this.x + (this.y*w)); 
};
Point.prototype.approach = function(other){
	return new Point(
			(this.x + other.x) / 2,
			(this.y + other.y) / 2
			);
}

function Sierp(w, h, step){
	var self = this;

	self.width = w;
	self.height = h;
	self.max = 0;
	self.step = step;
	self.output = null;

	self.points = new Array();

	self.xy = function(x, y){
		if (x.toIndex){	return x.toIndex(self.width); }
		return parseInt(x + (y * self.width));
	}
	self.getPoint = function(index){
		return new Point(index % self.width, index / self.width);
	}

	self.generation = function(from){
		var to = new Int16Array(self.width * self.height);
		self.max=0;
		var i = 0;
		for (var i = 0; i < from.length; ++i) {
			if (from[i] > 0){
				var pos = self.getPoint(i);
				for (var p = 0; p < self.points.length; ++p){
					var point  = self.points[p];
					var new_index = self.xy(pos.approach(point));
					to[new_index] = Math.min(to[new_index] + from[i], 32767);
					self.max = Math.max(self.max, to[new_index]);
				}
			}
		};
		return to;
	}

	self.set_points = function(sides){
		self.points = new Array();
		var full_circle = 2.0*Math.PI;
		var step_angle = full_circle/sides;
		for (var i=0; i<sides; ++i){
			var angle =(-full_circle/4) + step_angle * i;
			var new_x = 0.5 + Math.cos(angle) * 0.499;
			var new_y = 0.5 + Math.sin(angle) * 0.499;
			self.points.push(new Point(new_x * self.width,new_y * self.height));
		}
		self.reset();
	};

	self.reset = function(){
		self.output = new Int16Array(self.width * self.height);
		self.output[self.xy(self.width/2, self.height/2)] = self.step;
	}

	self.next = function(){
		self.output = self.generation(self.output);
	}

	self.render = function(canvas){
		var ctx = canvas.getContext('2d');
		var image = ctx.createImageData(self.width, self.height);
		var factor = 255/self.max;
		for (var i=0; i<self.output.length; i += 1){
			var basis = i * 4;
			image.data[basis] = 0;
			image.data[basis+1] = 0;
			image.data[basis+2] = 0;
			image.data[basis+3] = self.output[i] * factor;
		}
		ctx.putImageData(image, 0, 0);
//		exctx.putImageData(context.getImageData(0, 0, 200, 200), 0, 0);	

	}

}