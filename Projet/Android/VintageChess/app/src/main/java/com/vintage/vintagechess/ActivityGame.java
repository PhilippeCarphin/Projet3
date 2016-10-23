package com.vintage.vintagechess;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.drawable.BitmapDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;

public class ActivityGame extends AppCompatActivity implements View.OnTouchListener , View.OnClickListener{

    public static int X, Y;

    /*
    private enum Position {
        a1(0, 1),
        b1(-1, -1),
        c1(-1, 0),
        d1(1, 0),
        e1(1, 0),
        f1(1, 0),
        g1(1, 0),
        h1(1, 0),
        a2(0, 1),
        b2(-1, -1),
        c2(-1, 0),
        d2(1, 0),
        e2(1, 0),
        f2(1, 0),
        g2(1, 0),
        h2(1, 0),
        a7(0, 1),
        b7(-1, -1),
        c7(-1, 0),
        d7(1, 0),
        e7(1, 0),
        f7(1, 0),
        g7(1, 0),
        h7(1, 0),
        a8(0, 1),
        b8(-1, -1),
        c8(-1, 0),
        d8(1, 0),
        e8(1, 0),
        f8(1, 0),
        g8(1, 0),
        h8(1, 0);

        Direction(int i, int j) {
            X = i;
            Y = j;
        }
    }*/


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
    int startX;
    int startY;

    Bitmap testPiece;
    ImageView testPiece2;
    int testPieceX = 1342;
    int testPieceY = 127;
    Canvas tempCanvas;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_game);

        chessBoard = (ImageView) findViewById(R.id.imageView);
        chessBoard.setImageResource(android.R.color.transparent);


        Game.chessBoard = chessBoard;
        chessBoard.setOnTouchListener(this);

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

        undoColorFilter();

    }

    private int fieldImgXY[] = new int[2];
    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        Game.width = chessBoard.getWidth();
        Game.height = chessBoard.getHeight();
        drawBoard();

    }

    public void drawBoard() {

        Log.d("DRAW", "");

        Bitmap imagenAndroid = BitmapFactory.decodeResource(getResources(),R.drawable.bw_chess_board);
        imagenAndroid = Bitmap.createBitmap(imagenAndroid,0,0,2999,2999);
        tempCanvas = new Canvas(imagenAndroid);
        Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setColor(Color.BLUE);
        tempCanvas.drawRect(0, 0, 40, 40, paint);

        testPiece = BitmapFactory.decodeResource(getResources(),R.drawable.black_bishop_1);
        tempCanvas.drawBitmap(testPiece,testPieceX,testPieceY,null);
        drawPieces(tempCanvas);
        chessBoard.setImageDrawable(new BitmapDrawable(chessBoard.getResources(), imagenAndroid));
        chessBoard.setOnTouchListener(this);
    }

    public void drawPieces(Canvas can){

        //Bitmap blackPawn = BitmapFactory.decodeResource(getResources(),R.drawable.black_pawn_1);
        Bitmap whitePawn = BitmapFactory.decodeResource(getResources(),R.drawable.white_pawn_1);
        Bitmap whitebishop = BitmapFactory.decodeResource(getResources(),R.drawable.white_bishop_1);
        Bitmap whiteking = BitmapFactory.decodeResource(getResources(),R.drawable.white_king_1);
        Bitmap whiteknight= BitmapFactory.decodeResource(getResources(),R.drawable.white_knight_1);
        Bitmap whitequeen= BitmapFactory.decodeResource(getResources(),R.drawable.white_queen_1);
        Bitmap whiterook= BitmapFactory.decodeResource(getResources(),R.drawable.white_rook_1);


       // for(int i = 1; i<7; i++){}

        can.drawBitmap(whitePawn, 126+ 76 ,76,null);
        can.drawBitmap(whitePawn, 126+ 76 + 152 ,76 ,null);
        can.drawBitmap(whitePawn, 126+ 76 + 2*152,76,null);
        can.drawBitmap(whitePawn, 126+ 76 + 3*152 ,76 ,null);
        can.drawBitmap(whitePawn, 126+ 76 + 4*152,76 ,null);
        can.drawBitmap(whitePawn, 126+ 76 + 5*152,76 ,null);
        can.drawBitmap(whitePawn, 126+ 76 + 6*152,76 ,null);
        can.drawBitmap(whitePawn, 126+ 76 + 7*152,76 ,null);


    }
    @Override
    public boolean onTouch(View v, MotionEvent curent) {
        Log.d("DOWN", String.valueOf(1));

        switch (curent.getAction())
        {
            case MotionEvent.ACTION_DOWN:
                testPieceY = (int)curent.getY();
                testPieceX = (int)curent.getX();
                Log.d("testPieceY", String.valueOf(testPieceY));
                Log.d("testPieceX", String.valueOf(testPieceX));
                Log.d("DOWN", String.valueOf(1));

                break;
            case MotionEvent.ACTION_UP:
                testPieceY = (int)curent.getY();
                testPieceX = (int)curent.getX();
                Log.d("testPieceY", String.valueOf(testPieceY));
                Log.d("testPieceX", String.valueOf(testPieceX));
                Log.d("UP", String.valueOf(1));
                break;
            case MotionEvent.ACTION_MOVE:
                testPieceY = (int)curent.getY();
                testPieceX = (int)curent.getX();
                Log.d("MOVE", String.valueOf(1));
                break;
        }
        chessBoard.invalidate();
        drawBoard();
        return true;
    }

    /*Il reste encore du travail la dessus */
    @Override
    public void onClick(View v) {

        switch (v.getId())
        {
            case R.id.imageView1:
                chessBoard.setImageResource(R.drawable.red_chess_board);
                undoColorFilter();
                redStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView2:
                chessBoard.setImageResource(R.drawable.red_chess_board);
                undoColorFilter();
                redStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
               v.invalidate();
                break;
            case R.id.imageView3:
                chessBoard.setImageResource(R.drawable.bw_chess_board);
                undoColorFilter();
                bwStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView4:
                chessBoard.setImageResource(R.drawable.bw_chess_board);
                undoColorFilter();
                bwStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView5:
                chessBoard.setImageResource(R.drawable.blue_chess_board);
                undoColorFilter();
                blueStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView6:
                chessBoard.setImageResource(R.drawable.blue_chess_board);
                undoColorFilter();
                blueStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView7:
                chessBoard.setImageResource(R.drawable.wood_chess_board);
                undoColorFilter();
                woodStyle1.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
            case R.id.imageView8:
                chessBoard.setImageResource(R.drawable.wood_chess_board);
                undoColorFilter();
                woodStyle2.getDrawable().setColorFilter(0x77000000, PorterDuff.Mode.DARKEN);
                v.invalidate();
                break;
        }

    }

    public void undoColorFilter(){
        redStyle1.getDrawable().setColorFilter(null);
        redStyle2.getDrawable().setColorFilter(null);
        bwStyle1.getDrawable().setColorFilter(null);
        bwStyle2.getDrawable().setColorFilter(null);
        blueStyle1.getDrawable().setColorFilter(null);
        blueStyle2.getDrawable().setColorFilter(null);
        woodStyle1.getDrawable().setColorFilter(null);
        woodStyle2.getDrawable().setColorFilter(null);
    }



}









