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

  public abstract boolean isInside(double x, double y);

  public abstract void move(double x, double y);

  public abstract String[] getInputs();
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
  public boolean isInside(double input_x, double input_y) {
    double distance = Math.sqrt(Math.pow(input_x - x, 2) + Math.pow(input_y - y, 2));
    return distance <= radius;
  }

  @Override
  public void move(double input_x, double input_y) {
    this.x += input_x;
    this.y += input_y;
  }

  @Override
  public String[] getInputs() {
    return new String[] {
      "new", "circle", color, String.valueOf(x), String.valueOf(y), String.valueOf(radius)
    };
  }
}

// Ellipse class
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

  private double getSemiMajorAxis() {
    double distanceX = Math.abs(x2 - x1);
    double distanceY = Math.abs(y2 - y1);
    double distance = Math.sqrt(distanceX * distanceX + distanceY * distanceY);
    double leftOver = (d - distance) / 2;
    return leftOver + (distance / 2);
  }

  private double getSemiMinorAxis() {
    double distanceX = Math.abs(x2 - x1);
    double distanceY = Math.abs(y2 - y1);
    double distance = Math.sqrt(distanceX * distanceX + distanceY * distanceY);
    return Math.sqrt(Math.pow(d / 2, 2) - Math.pow(distance / 2, 2));
  }

  @Override
  public double getArea() {
    double a = getSemiMajorAxis();
    double b = getSemiMinorAxis();
    return Math.PI * a * b;
  }

  @Override
  public double getCircumference() {
    double a = getSemiMajorAxis();
    double b = getSemiMinorAxis();
    double h = Math.pow(a - b, 2) / Math.pow(a + b, 2);
    return Math.PI * (a + b) * (1 + (3 * h) / (10 + Math.sqrt(4 - 3 * h)));
  }

  @Override
  public boolean isInside(double input_x, double input_y) {
    double distance1 = Math.sqrt(Math.pow(input_x - x1, 2) + Math.pow(input_y - y1, 2));
    double distance2 = Math.sqrt(Math.pow(input_x - x2, 2) + Math.pow(input_y - y2, 2));
    return (distance1 + distance2) <= d;
  }

  @Override
  public void move(double input_x, double input_y) {
    this.x1 += input_x;
    this.y1 += input_y;
    this.x2 += input_x;
    this.y2 += input_y;
  }

  @Override
  public String[] getInputs() {
    return new String[] {
      "new",
      "Ellipse",
      color,
      String.valueOf(x1),
      String.valueOf(y1),
      String.valueOf(x2),
      String.valueOf(y2),
      String.valueOf(d)
    };
  }
}

// Parallelogram class
class Parallelogram extends Shape {
  private double x1;
  private double y1;
  private double x2;
  private double y2;
  private double x3;
  private double y3;
  private double x4;
  private double y4;

  public Parallelogram(
      int id, String color, double x1, double y1, double x2, double y2, double x3, double y3) {
    super(id, color);
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
    this.x3 = x3;
    this.y3 = y3;
    this.x4 = x2 + (x3 - x1);
    this.y4 = y2 + (y3 - y1);
  }

  @Override
  public double getArea() {
    Triangle triangle = new Triangle(0, "", x1, y1, x2, y2, x4, y4);
    return 2 * triangle.getArea();
  }

  @Override
  public double getCircumference() {
    double side1 = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
    double side2 = Math.sqrt(Math.pow(x2 - x3, 2) + Math.pow(y2 - y3, 2));
    return 2 * (side1 + side2);
  }

  @Override
  public boolean isInside(double input_x, double input_y) {
    Triangle triangle1 = new Triangle(0, "", x1, y1, x2, y2, x4, y4);
    Triangle triangle2 = new Triangle(0, "", x3, y3, x2, y2, x4, y4);
    return triangle1.isInside(input_x, input_y) || triangle1.isInside(input_x, input_y);
  }

  @Override
  public void move(double input_x, double input_y) {
    this.x1 += input_x;
    this.y1 += input_y;
    this.x2 += input_x;
    this.y2 += input_y;
    this.x3 += input_x;
    this.y3 += input_y;
  }

  @Override
  public String[] getInputs() {
    return new String[] {
      "new",
      "Parallelogram",
      color,
      String.valueOf(x1),
      String.valueOf(y1),
      String.valueOf(x2),
      String.valueOf(y2),
      String.valueOf(x3),
      String.valueOf(y3)
    };
  }
}

// Rectangle class
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
  public boolean isInside(double input_x, double input_y) {
    double maxX = Math.max(x1, x2);
    double minX = Math.min(x1, x2);
    double maxY = Math.max(y1, y2);
    double minY = Math.min(y1, y2);
    boolean insideX = (input_x >= minX) && (input_x <= maxX);
    boolean insideY = (input_y >= minY) && (input_y <= maxY);
    return insideX && insideY;
  }

  @Override
  public void move(double input_x, double input_y) {
    this.x1 += input_x;
    this.y1 += input_y;
    this.x2 += input_x;
    this.y2 += input_y;
  }

  @Override
  public String[] getInputs() {
    return new String[] {
      "new",
      "Rectangle",
      color,
      String.valueOf(x1),
      String.valueOf(y1),
      String.valueOf(x2),
      String.valueOf(y2)
    };
  }
}

// Square class
class Square extends Rectangle {
  private double length;

  public Square(int id, String color, double x1, double y1, double length) {
    super(id, color, x1 - length / 2, y1 - length / 2, x1 + length / 2, y1 + length / 2);
    this.length = length;
  }
}

// Triangle class
class Triangle extends Shape {
  private double x1;
  private double y1;
  private double x2;
  private double y2;
  private double x3;
  private double y3;

  public Triangle(
      int id, String color, double x1, double y1, double x2, double y2, double x3, double y3) {
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
    return Math.sqrt(
        semiPerimeter
            * (semiPerimeter - side1)
            * (semiPerimeter - side2)
            * (semiPerimeter - side3));
  }

  @Override
  public double getCircumference() {
    double side1 = Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));
    double side2 = Math.sqrt(Math.pow(x3 - x2, 2) + Math.pow(y3 - y2, 2));
    double side3 = Math.sqrt(Math.pow(x1 - x3, 2) + Math.pow(y1 - y3, 2));
    return side1 + side2 + side3;
  }

  @Override
  public boolean isInside(double input_x, double input_y) {
    double area1 = 0.5 * Math.abs((x1 - input_x) * (y2 - y1) - (x1 - x2) * (input_y - y1));
    double area2 = 0.5 * Math.abs((x2 - input_x) * (y3 - y2) - (x2 - x3) * (input_y - y2));
    double area3 = 0.5 * Math.abs((x3 - input_x) * (y1 - y3) - (x3 - x1) * (input_y - y3));
    double triangleArea = getArea();
    return Math.abs(triangleArea - (area1 + area2 + area3)) < 1e-10;
  }

  @Override
  public void move(double input_x, double input_y) {
    this.x1 += input_x;
    this.y1 += input_y;
    this.x2 += input_x;
    this.y2 += input_y;
    this.x3 += input_x;
    this.y3 += input_y;
  }

  @Override
  public String[] getInputs() {
    return new String[] {
      "new",
      "Triangle",
      color,
      String.valueOf(x1),
      String.valueOf(y1),
      String.valueOf(x2),
      String.valueOf(y2),
      String.valueOf(x3),
      String.valueOf(y3)
    };
  }
}

public class ToyCAD {
  private List<Shape> drawing;
  private int id = 0;

  public ToyCAD() {
    drawing = new ArrayList<>();
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

  private int createShape(String[] parts) {
    String shapeType = parts[1].toLowerCase();
    String color = parts[2].toUpperCase();

    Shape shape = null;

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
      id += 1;
      drawing.add(shape);
      System.out.println(shape.id);
      return shape.id;
    } else {
      System.out.println("Invalid shape type");
      return -1;
    }
  }

  private void deleteShape(int shapeId) {
    for (Shape shape : drawing) {
      if (shape.getId() == shapeId) {
        drawing.remove(shape);
        break;
      }
    }
  }

  private void moveShape(int shapeId, double moveX, double moveY) {
    for (Shape shape : drawing) {
      if (shape.getId() == shapeId) {
        shape.move(moveX, moveY);
        break;
      }
    }
  }

  private void copyShape(int shapeId, double moveX, double moveY) {
    for (Shape shape : drawing) {
      if (shape.getId() == shapeId) {
        String[] parts = shape.getInputs();
        int id = createShape(parts);
        moveShape(id, moveX, moveY);
        break;
      }
    }
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
        System.out.println(shape.isInside(x, y) ? 1 : 0);
        break;
      }
    }
  }

  public static void main(String[] args) {
    ToyCAD toy = new ToyCAD();

    // Read input commands from stdin
    Scanner scanner = new Scanner(System.in);
    while (scanner.hasNextLine()) {
      String command = scanner.nextLine();
      toy.processCommand(command);
    }
    scanner.close();
  }
}
