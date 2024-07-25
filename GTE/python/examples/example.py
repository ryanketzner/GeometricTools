import GeometricTools as gte

vector = gte.Vector3d([1.0,2.0,3.0]);
constant = 2.0;
halfspace = gte.Halfspace3d(vector,constant);
sphere = gte.Sphere3d(vector,constant);
