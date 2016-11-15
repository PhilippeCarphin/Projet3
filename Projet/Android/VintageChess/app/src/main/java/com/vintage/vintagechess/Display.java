package com.vintage.vintagechess;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.BitmapDrawable;
import android.media.Image;
import android.util.TypedValue;
import android.widget.ImageView;

/**
 * Created by User on 2016-11-11.
 */

public class Display {
    private double offset;
    private Game game;

    public Display(Game game) {
        offset = 0.05133;
        this.game = game;
    }


    //draws the board and its pieces
    public void drawFullBoard() {
        drawBoard();
        drawMotionlessPieces();
    }

    //draws the chessboard image
    public void drawBoard() {

        Bitmap imageAndroid = Bitmap.createBitmap(game.activityGame.board.getHeight(), game.activityGame.board.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas tempCanvas = new Canvas(imageAndroid);
        //tempCanvas.drawBitmap(boardImg,0,0,null);
        //display the first square
        Paint backPaint = new Paint();
        backPaint.setColor(BoardColors.borderColor);
        tempCanvas.drawRect(0, 0, game.activityGame.board.getHeight(), game.activityGame.board.getHeight(), backPaint);

        //display the squares
        for (int i = 0 ; i < 8 ; i++) {
            for (int j = 0 ; j < 8 ; j++) {
                int left = getPixelPosition(i);
                int right = getPixelPosition(i+1);
                int top = getPixelPosition(7-j);
                int bottom = getPixelPosition(7-j+1);
                int c;

                if ((game.lastMoveOrigin != null) &&((i == game.lastMoveOrigin.x && j == game.lastMoveOrigin.y) || (i == game.lastMoveDestination.x && j == game.lastMoveDestination.y))) {
                    c = BoardColors.lastSquareColor;
                }
                else {
                    c = (((i + j) % 2) == 1) ? GameConfig.darkSquareColor : GameConfig.lightSquareColor;
                }
                Paint squarePaint = new Paint();
                squarePaint.setColor(c);

                tempCanvas.drawRect((float)left, (float)top, (float)right, (float)bottom, squarePaint);
            }
        }


        //display the numbers
        Paint numberPaint = new Paint(Paint.ANTI_ALIAS_FLAG | Paint.DITHER_FLAG);
        numberPaint.setColor(BoardColors.numberColor);
        numberPaint.setTextSize((int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, 12, Utilities.currentActivity.getResources().getDisplayMetrics()));
        numberPaint.setTextAlign(Paint.Align.CENTER);
        Paint.FontMetrics metric = numberPaint.getFontMetrics();
        int textHeight = (int) Math.ceil(metric.descent - metric.ascent);
        for (int i = 0 ; i < 8 ; i++) {
            String charToDraw = String.valueOf((char)(i+65));
            int x = getBoardOffset() + (int)(getSquareWidth() * (i + 0.5));
            int y = (int)(textHeight - metric.descent) + getBoardOffset()/2 + (int)((numberPaint.descent() + numberPaint.ascent()) / 2);
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            y = y + 8*getSquareWidth() + getBoardOffset();
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            charToDraw = "" + (8-i);
            x = getBoardOffset()/2;
            y = (int)(textHeight/2 - metric.descent) + getBoardOffset() + (int)(getSquareWidth() * (i+0.5));
            tempCanvas.drawText(charToDraw, x, y, numberPaint);
            x += getSquareWidth() * 8 + getBoardOffset();
            tempCanvas.drawText(charToDraw, x, y, numberPaint);

        }
        //tempCanvas.drawText("allo", 0, 0, numberPaint);

        game.activityGame.board.setImageDrawable(new BitmapDrawable(game.activityGame.board.getResources(), imageAndroid));

    }



    //draws the pieces that are not selected in the appropriate board
    public void drawMotionlessPieces() {
        //int id = activityGame.getResources().getIdentifier(board_style + "_chess_board", "drawable", activityGame.getPackageName());
          Bitmap imageAndroid = Bitmap.createBitmap(game.activityGame.motionlessPieces.getHeight(), game.activityGame.motionlessPieces.getHeight(), Bitmap.Config.ARGB_8888);
        //boardImg.copy(boardImg.getConfig(), true);
        Canvas tempCanvas = new Canvas(imageAndroid);
        for(Piece[] array : game.pieces) {
            for (Piece p : array)  {
                if (p != null) {
                    Bitmap imageBitmap = p.getBitmap(game.activityGame.board, offset);
                    Point position = p.p_;
                    int xPos = getPixelPosition(position.x);
                    int yPos = getPixelPosition(7 - position.y);
                    tempCanvas.drawBitmap(imageBitmap, xPos, yPos, null);
                }
            }
        }

        game.activityGame.motionlessPieces.setImageDrawable(new BitmapDrawable(game.activityGame.motionlessPieces.getResources(), imageAndroid));
        //board.setOnTouchListener(activityGame);
    }

    //draw the curent moving piece in the appropriate view
    public void drawMovingPiece() {
        Bitmap imageAndroid = Bitmap.createBitmap(game.activityGame.movingPiece.getHeight(), game.activityGame.movingPiece.getHeight(), Bitmap.Config.ARGB_8888);

        if (game.currentPiece != null) {
            Canvas tempCanvas = new Canvas(imageAndroid);
            Bitmap imageBitmap = game.currentPiece.getBitmap(game.activityGame.board, offset);
            Point position = game.currentPiece.p_;
            int xPos =  getPixelPosition(position.x);
            int yPos =  getPixelPosition(7-position.y);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
            tempCanvas.drawBitmap(imageBitmap,xPos,yPos,null);
        }

        game.activityGame.movingPiece.setImageDrawable(new BitmapDrawable(game.activityGame.movingPiece.getResources(), imageAndroid));
    }

    //get the size in pixels of a square in the board
    public int getSquareWidth() {
        return (int)((1-2*offset)*game.activityGame.board.getHeight()/8.0);
    }

    //get the size of the board border in pixels
    public int getBoardOffset() {
        return (int)(game.activityGame.board.getHeight() * offset);
    }

    //get the pixel position of the beginning of the square d
    public int getPixelPosition(int d) {
        return getBoardOffset() + d*getSquareWidth();
    }

    //todo
    public static boolean isOutOfBounds(Point p) {
        return false;
    }


    //convertir les coordonnees de pixel en coordonnees dans la grille de l'echiquier
    public Point getboardCoordinates(ImageView board, int xPix, int yPix) throws Exception {
        int x, y, xView, yView;
        int min, max;

        xView = xPix - (board.getWidth()-board.getHeight())/2;
        yView = yPix;
        min = getBoardOffset();
        max = board.getHeight() - getBoardOffset();
        if (xView < min || xView > max || yView < min || yView > max) {
            throw new Exception("Finger is out of bounds");
        }
        float insideWidth = board.getHeight()-getBoardOffset()*2;
        x = (int)(8*(((float)(xView - getBoardOffset()))/insideWidth));
        y = (int)(8*(((float)(yView - getBoardOffset()))/insideWidth));
        return new Point(x, 7-y);
    }


    //block the chessboard view
    public void blockBoard(ImageView board) {
        board.setEnabled(false);
    }

    //unblock the chessboard view
    public void unBlockBoard(ImageView board) {
        board.setEnabled(true);
    }





}
