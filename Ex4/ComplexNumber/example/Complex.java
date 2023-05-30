package com.example;

public class Complex {
    private static final double EPSILON = 0.001;
    private double realPart;
    private double imaginaryPart;

    public Complex(double realPart, double imaginaryPart) {
        this.realPart = realPart;
        this.imaginaryPart = imaginaryPart;
    }

    public Complex plus(Complex other) {
        double sumReal = this.realPart + other.realPart;
        double sumImaginary = this.imaginaryPart + other.imaginaryPart;
        return new Complex(sumReal, sumImaginary);
    }

    public Complex minus(Complex other) {
        double diffReal = this.realPart - other.realPart;
        double diffImaginary = this.imaginaryPart - other.imaginaryPart;
        return new Complex(diffReal, diffImaginary);
    }

    public Complex times(Complex other) {
        double productReal = (this.realPart * other.realPart) - (this.imaginaryPart * other.imaginaryPart);
        double productImaginary = (this.realPart * other.imaginaryPart) + (this.imaginaryPart * other.realPart);
        return new Complex(productReal, productImaginary);
    }

    public Complex divide(Complex other) {
        double divisor = Math.pow(other.realPart, 2) + Math.pow(other.imaginaryPart, 2);
        double quotientReal = ((this.realPart * other.realPart) + (this.imaginaryPart * other.imaginaryPart)) / divisor;
        double quotientImaginary = ((this.imaginaryPart * other.realPart) - (this.realPart * other.imaginaryPart)) / divisor;
        return new Complex(quotientReal, quotientImaginary);
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

    public boolean almostEquals(Complex other) {
        double diffReal = this.realPart - other.realPart;
        double diffImaginary = this.imaginaryPart - other.imaginaryPart;
        double diffRadius = Math.sqrt(Math.pow(diffReal, 2) + Math.pow(diffImaginary, 2));
        return diffRadius < EPSILON;
    }
}


