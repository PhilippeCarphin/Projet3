package com.vintage.vintagechess;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.util.Pair;

import java.util.Map;

/**
 * Created by Sabrina on 2016-10-25.
 */

public class Piece extends AppCompatActivity {


    public static int largeur = 152;
    public static int midLargeur = 152/2;
    public int xPosition_; // creer une map prochainement
    public int yPosition_;
    public String symboleXY_;
    public String style_;
    public String color_;
    public int id_; // drawabale
    public int identifier_;

    public Piece(int id, int identifier, String style, String color, String position ){
        id_ = id ;
        style_ = style;
        color_ = color;
        symboleXY_ = position;
        identifier_ = identifier;
    }

    public void setPosition(int x, int y)
    {
        xPosition_ = x;
        yPosition_ = y;
    }

    public int getxPosition()
    {

        xPosition_ = SymboleToXY(symboleXY_).first;

        return xPosition_ ;
    }

    public int getyPosition()
    {
        yPosition_ = SymboleToXY(symboleXY_).second;
        return yPosition_ ;
    }

    public int getId_()
    {
        return id_;
    }

    public Pair<Integer,Integer> SymboleToXY(String position)
    {
        int i =  Integer.valueOf(position.charAt(0)) - 97;
        int x = 131 + (i*largeur + midLargeur);
        int j = 8 - Integer.valueOf(position.charAt(1))+48; // 48 cest le 1 en ascii
        int y = (j*largeur + midLargeur) + 67;
        Pair<Integer,Integer> xy = new Pair<>(x,y);

        Log.d("symbole _ X_Y", position +" "+String.valueOf(x) + " " + String.valueOf(y));
        return xy;
    }

}
