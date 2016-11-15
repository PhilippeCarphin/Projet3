package com.vintage.vintagechess;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Point;
import android.widget.ImageView;

/**
 * Created by User on 2016-11-01.
 */

public class Piece {
    public Point p_;
    public boolean isWhite_;
    public String type_;

    public Piece(Piece piece) {
        p_ = piece.p_;
        isWhite_ = piece.isWhite_;
        type_ = piece.type_;
    }

    public Piece(String type, int x, int y, boolean isWhite) {
        p_ = new Point(x,y);
        isWhite_ = isWhite;
        type_ = type;
    }

    public int getResourceID() {
        String resourceName = (isWhite_ ? "white" : "black") + "_"+ type_ + "_" +GameConfig.style;  // where myresource (without the extension) is the file
        Context context = Utilities.currentActivity;
        return context.getResources().getIdentifier(resourceName, "drawable", context.getPackageName());
    }

    public static int getResourceID(String resourceName) {
        Context context = Utilities.currentActivity;
        return context.getResources().getIdentifier(resourceName, "drawable", context.getPackageName());
    }

    public static Bitmap getResourceBitmap(int resourceID, int width) {
        Bitmap bmp = BitmapFactory.decodeResource( Utilities.currentActivity.getResources(), resourceID );
        Bitmap img = Bitmap.createScaledBitmap( bmp, width, width, true );
        bmp.recycle();

        //Log.d("Id", String.valueOf(id));
        return img;
    }

    public static Bitmap getResourceBitmap(String resource, int width) {
        return getResourceBitmap(getResourceID(resource), width);
    }



    public Bitmap getBitmap(ImageView board, double offset) {
        Bitmap bmp = BitmapFactory.decodeResource( Utilities.currentActivity.getResources(), getResourceID() );
        int w = (int)(board.getHeight() - board.getHeight()*offset*2)/8;
        Bitmap img = Bitmap.createScaledBitmap( bmp, w, w, true );
        bmp.recycle();

        //Log.d("Id", String.valueOf(id));
        return img;
    }


}
