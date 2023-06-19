import java.util.*;

// Shape class represents a generic shape
abstract class Shape {
    protected int id;
    protected String color;

    public Shape(int id, String color) {
        this.id = id;
        this.color = color;
    }
    
    public int getId() {
        return id;
    }
    
    public String getColor() {
        return color;
    }
    
    public void setColor(String color) {
    	this.color = color;
    }
    
    public abstract double getArea();
    public abstract double getCircumference();
    public abstract void isInside(double x, double y);
}

// Circle class
class Circle extends Shape {
    private double x;
    private double y;
    private double radius;

    public Circle(int id, String color, double x, double y, double radius) {
        super(id, color);
        this.x = x;
        this.y = y;
        this.radius = radius;
    }
    
    @Override
    public double getArea() {
        return Math.PI * radius * radius;
    }
    
    @Override
    public double getCircumference() {
        return 2 * Math.PI * radius;
    }
    
    @Override
    public void isInside(double input_x, double input_y) {
       double distance = Math.sqrt(Math.pow(input_x - x, 2) + Math.pow(input_y - y, 2));
       System.out.println(distance <= radius ? 1 : 0);
    }
}

//Ellipse class
class Ellipse extends Shape {
	private double x1;
	private double y1;
	private double x2;
	private double y2;
	private double d;

	public Ellipse(int id, String color, double x1, double y1, double x2, double y2, double d) {
		super(id, color);
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
		this.d = d;
	}
    
	@Override
	public double getArea() {
		double semiMajorAxis = Math.abs(x2 - x1) / 2;
		double semiMinorAxis = Math.abs(y2 - y1) / 2;
		return Math.PI * semiMajorAxis * semiMinorAxis;
	}
	
	@Override
    public double getCircumference() {
        double semiMajorAxis = Math.abs(x2 - x1) / 2;
        double semiMinorAxis = Math.abs(y2 - y1) / 2;
        double h = Math.pow(semiMajorAxis - semiMinorAxis, 2) / Math.pow(semiMajorAxis + semiMinorAxis, 2);
        return Math.PI * (semiMajorAxis + semiMinorAxis) * (1 + (3 * h) / (10 + Math.sqrt(4 - 3 * h)));
    }
	
	@Override
    public void isInside(double input_x, double input_y) {
		double distance1 = Math.sqrt(Math.pow(input_x - x1, 2) + Math.pow(input_y - y1, 2));
		double distance2 = Math.sqrt(Math.pow(input_x - x2, 2) + Math.pow(input_y - y2, 2));
        System.out.println(distance1 + distance2 <= d ? 1 : 0);
    }
}

//Parallelogram class
class Parallelogram extends Shape {
	private double x1;
	private double y1;
	private double x2;
	private double y2;
	private double x3;
	private double y3;

	public Parallelogram(int id, String color, double x1, double y1, double x2, double y2, double x3, double y3) {
	   super(id, color);
	   this.x1 = x1;
	   this.y1 = y1;
	   this.x2 = x2;
	   this.y2 = y2;
	   this.x3 = x3;
	   this.y3 = y3;
	}
    
    @Override
    public double getArea() {
        double base = Math.abs(x3 - x1);
        double height = Math.abs(y3 - y1);
        return base * height;
    }

    @Override
    public double getCircumference() {
        double side1 = Math.sqrt(Math.pow(x3 - x1, 2) + Math.pow(y3 - y1, 2));
        double side2 = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
        return 2 * (side1 + side2);
    }
    
    @Override
    public void isInside(double input_x, double input_y) {
    
    }

//Rectangle class
class Rectangle extends Shape {
	private double x1;
	private double y1;
	private double x2;
	private double y2;

	public Rectangle(int id, String color, double x1, double y1, double x2, double y2) {
		super(id, color);
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
	}
    
	@Override
    public double getArea() {
        double length = Math.abs(x2 - x1);
        double width = Math.abs(y2 - y1);
        return length * width;
    }
	
	@Override
    public double getCircumference() {
        double width = Math.abs(x2 - x1);
        double height = Math.abs(y2 - y1);
        return 2 * (width + height);
    }
	
	@Override
    public void isInside(double input_x, double input_y) {
    
    }
}

//Square class
class Square extends Shape {
	private double x1;
	private double y1;
	private double length;

	public Square(int id, String color, double x1, double y1, double length) {
		super(id, color);
		this.x1 = x1;
		this.y1 = y1;
		this.length = length;
	}
    
	@Override
	public double getArea() {
		return length * length;
	}
	
	@Override
    public double getCircumference() {
        return 4 * length;
    }
	
	@Override
    public void isInside(double input_x, double input_y) {
    
    }
}

//Triangle class
class Triangle extends Shape {
	 private double x1;
	 private double y1;
	 private double x2;
	 private double y2;
	 private double x3;
	 private double y3;
	
	 public Triangle(int id, String color, double x1, double y1, double x2, double y2, double x3, double y3) {
	     super(id, color);
	     this.x1 = x1;
	     this.y1 = y1;
	     this.x2 = x2;
	     this.y2 = y2;
	     this.x3 = x3;
	     this.y3 = y3;
	 }
	    
	 @Override
	 public double getArea() {
		 // Using Heron's formula to calculate the area of a triangle
		 double side1 = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
		 double side2 = Math.sqrt(Math.pow(x3 - x2, 2) + Math.pow(y3 - y2, 2));
		 double side3 = Math.sqrt(Math.pow(x1 - x3, 2) + Math.pow(y1 - y3, 2));
	
		 double semiPerimeter = (side1 + side2 + side3) / 2;
		 return Math.sqrt(semiPerimeter * (semiPerimeter - side1) * (semiPerimeter - side2) * (semiPerimeter - side3));
	 }
	 
	 @Override
	 public double getCircumference() {
		 double side1 = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
         double side2 = Math.sqrt(Math.pow(x3 - x2, 2) + Math.pow(y3 - y2, 2));
         double side3 = Math.sqrt(Math.pow(x1 - x3, 2) + Math.pow(y1 - y3, 2));
         return side1 + side2 + side3;
     }
	 
	 @Override
	 public void isInside(double input_x, double input_y) {
    
     }
}



public class AutoCADToy {
    private List<Shape> drawing;

    public AutoCADToy() {
        drawing = new ArrayList<>();
        colorAreas = new HashMap<>();
    }

    public void processCommand(String command) {
        String[] parts = command.split(" ");
        String action = parts[0].toLowerCase();

        switch (action) {
            case "new":
                createShape(parts);
                break;
            case "delete":
                int shapeIdToDelete = Integer.parseInt(parts[1]);
                deleteShape(shapeIdToDelete);
                break;
            case "move":
                int shapeIdToMove = Integer.parseInt(parts[1]);
                double moveX = Double.parseDouble(parts[2]);
                double moveY = Double.parseDouble(parts[3]);
                moveShape(shapeIdToMove, moveX, moveY);
                break;
            case "copy":
                int shapeIdToCopy = Integer.parseInt(parts[1]);
                double copyX = Double.parseDouble(parts[2]);
                double copyY = Double.parseDouble(parts[3]);
                copyShape(shapeIdToCopy, copyX, copyY);
                break;
            case "area":
                String colorForArea = parts[1].toUpperCase();
                calculateColorArea(colorForArea);
                break;
            case "color":
                String colorForChange = parts[1].toUpperCase();
                int shapeIdForColor = Integer.parseInt(parts[2]);
                changeColor(shapeIdForColor, colorForChange);
                break;
            case "circumference":
                String colorForCircumference = parts[1].toUpperCase();
                calculateColorCircumference(colorForCircumference);
                break;
            case "is_inside":
                int shapeIdForCheck = Integer.parseInt(parts[1]);
                double xForCheck = Double.parseDouble(parts[2]);
                double yForCheck = Double.parseDouble(parts[3]);
                isInside(shapeIdForCheck, xForCheck, yForCheck);
                break;
            case "exit":
                System.exit(0);
                break;
            default:
                System.out.println("Invalid command.");
        }
    }


    private void createShape(String[] parts) {
        String shapeType = parts[1].toLowerCase();
        String color = parts[2].toUpperCase();

        Shape shape = null;
        int id = drawing.size();

        if (shapeType.equals("circle")) {
            double x = Double.parseDouble(parts[3]);
            double y = Double.parseDouble(parts[4]);
            double radius = Double.parseDouble(parts[5]);
            shape = new Circle(id, color, x, y, radius);
        }
        
        if (shapeType.equals("ellipse")) {
            double x1 = Double.parseDouble(parts[3]);
            double y1 = Double.parseDouble(parts[4]);
            double x2 = Double.parseDouble(parts[5]);
            double y2 = Double.parseDouble(parts[6]);
            double d = Double.parseDouble(parts[7]);
            shape = new Ellipse(id, color, x1, y1, x2, y2, d);
        }
        
        if (shapeType.equals("parallelogram")) {
            double x1 = Double.parseDouble(parts[3]);
            double y1 = Double.parseDouble(parts[4]);
            double x2 = Double.parseDouble(parts[5]);
            double y2 = Double.parseDouble(parts[6]);
            double x3 = Double.parseDouble(parts[7]);
            double y3 = Double.parseDouble(parts[8]);
            shape = new Parallelogram(id, color, x1, y1, x2, y2, x3, y3);
        }
        
        if (shapeType.equals("rectangle")) {
            double x1 = Double.parseDouble(parts[3]);
            double y1 = Double.parseDouble(parts[4]);
            double x2 = Double.parseDouble(parts[5]);
            double y2 = Double.parseDouble(parts[6]);
            shape = new Rectangle(id, color, x1, y1, x2, y2);
        }
        
        if (shapeType.equals("square")) {
            double x1 = Double.parseDouble(parts[3]);
            double y1 = Double.parseDouble(parts[4]);
            double length = Double.parseDouble(parts[5]);
            shape = new Square(id, color, x1, y1, length);
        }
        
        if (shapeType.equals("triangle")) {
            double x1 = Double.parseDouble(parts[3]);
            double y1 = Double.parseDouble(parts[4]);
            double x2 = Double.parseDouble(parts[5]);
            double y2 = Double.parseDouble(parts[6]);
            double x3 = Double.parseDouble(parts[7]);
            double y3 = Double.parseDouble(parts[8]);
            shape = new Triangle(id, color, x1, y1, x2, y2, x3, y3);
        }

        if (shape != null) {
            drawing.add(shape);
            System.out.println(shape.id);
        } else {
            System.out.println("Invalid shape type");
        }
    }

    private void deleteShape(int shapeId) {
        for (Shape shape : drawing) {
            if (shape.getId() == shapeId) {
            	drawing.remove(shapeToDelete);
                break;
            }
        }
    }

    private void moveShape(int shapeId, double moveX, double moveY) {
        // Find the shape in the drawing list based on shapeId
        // Move the shape by updating its coordinates
    }

    private void copyShape(int shapeId, double copyX, double copyY) {
        // Find the shape in the drawing list based on shapeId
        // Create a new copy of the shape at the given coordinates
        // Add the new shape to the drawing list
        // Print the ID of the newly created shape
    }

    private void calculateColorArea(String color) {
        double totalArea = 0;
        for (Shape shape : drawing) {
            if (shape.getColor().equals(color)) {
                totalArea += shape.getArea();
            }
        }
        System.out.printf("%.2f%n", totalArea); 
    }
    
    private void calculateColorCircumference(String color) {
    	double totalCircumference = 0;
        for (Shape shape : drawing) {
            if (shape.getColor().equals(color)) {
            	totalCircumference += shape.getCircumference();
            }
        }
        System.out.printf("%.2f%n", totalCircumference); 
    }
    
    private void changeColor(int shapeId, String color) {
        for (Shape shape : drawing) {
            if (shape.getId() == shapeId) {
            	shape.setColor(color);
                break;
            }
        }

    }
    
    private void isInside(int shapeId, double x, double y) {
        for (Shape shape : drawing) {
            if (shape.getId() == shapeId) {
            	shape.isInside(x, y);
            	break;
            }
        }
    }

    
    public static void main(String[] args) {
        AutoCADToy toy = new AutoCADToy();

        // Read input commands from stdin
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            String command = scanner.nextLine();
            toy.processCommand(command);
        }
        scanner.close();
    }
}
