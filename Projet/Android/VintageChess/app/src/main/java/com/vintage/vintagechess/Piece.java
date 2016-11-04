package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;

/**
 * Created by User on 2016-11-01.
 */

public class Piece {
    public Point p_;
    public boolean isWhite_;
    public String type_;
    public Piece(String type, int x, int y, boolean isWhite) {
        p_ = new Point(x,y);
        isWhite_ = isWhite;
        type_ = type;
    }

    public int getResourceID() {
        String resourceName = (isWhite_ ? "white" : "black") + "_"+ type_ + "_" +Game.style;  // where myresource (without the extension) is the file
        Context context = Game.activityGame;
        return context.getResources().getIdentifier(resourceName, "drawable", context.getPackageName());
    }



    public Bitmap getBitmap() {
        Bitmap bmp = BitmapFactory.decodeResource( Game.activityGame.getResources(), getResourceID() );
        int w = (int)(Game.board.getHeight() - Game.board.getHeight()*Game.offset*2)/8;
        Bitmap img = Bitmap.createScaledBitmap( bmp, w, w, true );
        bmp.recycle();

        //Log.d("Id", String.valueOf(id));
        return img;
    }


}
