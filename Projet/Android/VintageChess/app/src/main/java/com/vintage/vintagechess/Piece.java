package com.vintage.vintagechess;

import java.util.Map;

/**
 * Created by PO on 2016-10-25.
 */

public class Piece {

    public enum Position {
        x,
        y
    }

    public double xPosition_; // creer une map prochainement
    public double yPosition_;
    public Map<String, Position > position_;
    public String style_;
    public String color_;
    public int id_; // drawabale
    public int identifier_;

    public Piece(int id, int identifier, String style, String color, int xPosition, int yPosition){
        id_ = id ;
        style_ = style;
        color_ = color;
        xPosition_ = xPosition;
        yPosition_ = yPosition;
        identifier_ = identifier;

    }

    public void setPosition(double x, double y)
    {
        xPosition_ = x;
        yPosition_ = y;
    }

    public double getxPosition()
    {
        return xPosition_ ;
    }

    public double getyPosition()
    {
        return yPosition_ ;
    }

    public int getId_()
    {
        return id_;
    }
}
