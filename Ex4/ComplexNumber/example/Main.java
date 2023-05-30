package com.example;

public class Main {
    public static void main(String[] args) {
        Complex complex1 = new Complex(2.5, 1.5);
        Complex complex2 = new Complex(1.0, 0.5);

        Complex sum = complex1.plus(complex2);
        System.out.println("Sum: " + sum.getRealPart() + " + " + sum.getImaginaryPart() + "i");

        Complex difference = complex1.minus(complex2);
        System.out.println("Difference: " + difference.getRealPart() + " + " + difference.getImaginaryPart() + "i");

        Complex product = complex1.times(complex2);
        System.out.println("Product: " + product.getRealPart() + " + " + product.getImaginaryPart() + "i");

        Complex quotient = complex1.divide(complex2);
        System.out.println("Quotient: " + quotient.getRealPart() + " + " + quotient.getImaginaryPart() + "i");

        System.out.println("Radius of complex1: " + complex1.getRadius());
        System.out.println("Argument of complex1: " + complex1.getArgument());

        boolean isAlmostEquals = complex1.almostEquals(complex2);
        System.out.println("complex1 almost equals complex2: " + isAlmostEquals);
    }
}
