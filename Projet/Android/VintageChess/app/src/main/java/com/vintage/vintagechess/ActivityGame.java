package com.vintage.vintagechess;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;

import java.util.LinkedList;
import java.util.List;

public class ActivityGame extends AppCompatActivity implements View.OnClickListener {

    private static int a = 0;
    ImageView chessBoard;
    ImageView display;
    ImageView redStyle1;
    ImageView redStyle2;
    ImageView bwStyle1;
    ImageView bwStyle2;
    ImageView blueStyle1;
    ImageView blueStyle2;
    ImageView woodStyle1;
    ImageView woodStyle2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        chessBoard = (ImageView) findViewById(R.id.imageView);
        chessBoard.setImageResource(android.R.color.transparent);


        Game.chessBoard = chessBoard;
        //String uri = "@drawable/blabla";
        //Game.activityGame = this;


        //Utilities.messageBox(Integer.valueOf(Game.width).toString(), "", this);
        //Game.drawBoard();

        display = (ImageView) findViewById(R.id.imageView);
        redStyle1 = (ImageView) findViewById(R.id.imageView1);
        redStyle2 =(ImageView) findViewById(R.id.imageView2);
        bwStyle1 = (ImageView) findViewById(R.id.imageView3);
        bwStyle2 =(ImageView) findViewById(R.id.imageView4);
        blueStyle1 = (ImageView) findViewById(R.id.imageView5);
        blueStyle2 =(ImageView) findViewById(R.id.imageView6);
        woodStyle1 =(ImageView) findViewById(R.id.imageView7);
        woodStyle2 =(ImageView) findViewById(R.id.imageView8);

        redStyle1.setOnClickListener(this);
        redStyle2.setOnClickListener(this);
        bwStyle1.setOnClickListener(this);
        bwStyle2.setOnClickListener(this);
        blueStyle1.setOnClickListener(this);
        blueStyle2.setOnClickListener(this);
        woodStyle1.setOnClickListener(this);
        woodStyle2.setOnClickListener(this);

    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Game.width = chessBoard.getWidth();
        Game.height = chessBoard.getHeight();
        Game.drawBoard();
    }


    /*Il reste encore du travail la dessus */
    @Override
    public void onClick(View v) {

        switch (v.getId())
        {
            case R.id.imageView1:
                display.setImageResource(R.drawable.red_square_with_style1);
                redStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView2:
                display.setImageResource(R.drawable.red_square_with_style2);
                redStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
               v.invalidate();
                break;
            case R.id.imageView3:
                display.setImageResource(R.drawable.black_and_white_square_with_style1);
                bwStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView4:
                display.setImageResource(R.drawable.black_and_white_square_with_style2);
                bwStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView5:
                display.setImageResource(R.drawable.blue_square_with_style1);
                blueStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView6:
                display.setImageResource(R.drawable.blue_square_with_style2);
                blueStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView7:
                display.setImageResource(R.drawable.wood_square_with_style1);
                woodStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView8:
                display.setImageResource(R.drawable.wood_square_with_style2);
                woodStyle2.isHovered();
                woodStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
        }

    }
}









