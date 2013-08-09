// Parameters

// Main box
w = 50;
l = 70;
h = 15;
thickness = 2; 

// Sensor
sensor_w = 25.4; 
sensor_l = 25.4;
distance_from_top = 10;

// Clips
attach_clip_w = 5;
attach_clip_l = 2;
distance_from_edge = h/2 - attach_clip_l;

// Front box
difference(){

	difference(){
		cube([h,w,l]);
		translate([-thickness,thickness,thickness]) {
  			cube([h,w-2*thickness,l-2*thickness]);
		}
	}

	// Clips on the sides
	translate([distance_from_edge,-5,l/2]) {
		cube([attach_clip_l,60,attach_clip_w]);
	} 
	
}