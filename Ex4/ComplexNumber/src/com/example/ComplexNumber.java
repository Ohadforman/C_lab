public class ComplexNumber {
    private static final double EPSILON = 0.001;
    private double realPart;
    private double imaginaryPart;

    public ComplexNumber(double realPart, double imaginaryPart) {
        this.realPart = realPart;
        this.imaginaryPart = imaginaryPart;
    }

    public ComplexNumber plus(ComplexNumber other) {
        double sumReal = this.realPart + other.realPart;
        double sumImaginary = this.imaginaryPart + other.imaginaryPart;
        return new ComplexNumber(sumReal, sumImaginary);
    }

    public ComplexNumber minus(ComplexNumber other) {
        double diffReal = this.realPart - other.realPart;
        double diffImaginary = this.imaginaryPart - other.imaginaryPart;
        return new ComplexNumber(diffReal, diffImaginary);
    }

    public ComplexNumber times(ComplexNumber other) {
        double productReal = (this.realPart * other.realPart) - (this.imaginaryPart * other.imaginaryPart);
        double productImaginary = (this.realPart * other.imaginaryPart) + (this.imaginaryPart * other.realPart);
        return new ComplexNumber(productReal, productImaginary);
    }

    public ComplexNumber divide(ComplexNumber other) {
        double divisor = Math.pow(other.realPart, 2) + Math.pow(other.imaginaryPart, 2);
        double quotientReal = ((this.realPart * other.realPart) + (this.imaginaryPart * other.imaginaryPart)) / divisor;
        double quotientImaginary = ((this.imaginaryPart * other.realPart) - (this.realPart * other.imaginaryPart)) / divisor;
        return new ComplexNumber(quotientReal, quotientImaginary);
    }

    public double getRealPart() {
        return realPart;
    }

    public double getImaginaryPart() {
        return imaginaryPart;
    }

    public double getRadius() {
        return Math.sqrt(Math.pow(realPart, 2) + Math.pow(imaginaryPart, 2));
    }

    public double getArgument() {
        return Math.atan2(imaginaryPart, realPart);
    }

    public boolean almostEquals(ComplexNumber other) {
        double diffReal = this.realPart - other.realPart;
        double diffImaginary = this.imaginaryPart - other.imaginaryPart;
        double diffRadius = Math.sqrt(Math.pow(diffReal, 2) + Math.pow(diffImaginary, 2));
        return diffRadius < EPSILON;
    }

    public ComplexNumber fromPolarCoordinates(double radius, double argument) {
        double real = radius * Math.cos(argument);
        double imaginary = radius * Math.sin(argument);
        return new ComplexNumber(real, imaginary);
    }
}


