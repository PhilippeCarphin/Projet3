package com.vintage.vintagechess;

import android.content.res.Resources;
import android.os.AsyncTask;
import android.util.Log;

import org.json.JSONException;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.Reader;
import java.io.UnsupportedEncodingException;
import java.net.HttpURLConnection;
import java.net.SocketTimeoutException;
import java.net.URL;

/**
 * Created by User on 2016-11-03.
 */

class RequestTask extends AsyncTask<String, String, String> {

    private RequestCallback onSucess;
    private RequestCallback onFail;
    private boolean receiveJSON;
    private static ActivityCreateGame activityCreateGame;
    private String suffix;
    private String method;
    private String body;

    //constructor
    public RequestTask(RequestCallback onSucessCallBack,RequestCallback onFailCallback , boolean receiveJSON, String suffix, String method, String body) {
        super();
        this.receiveJSON = receiveJSON;
        this.suffix = suffix;
        this.method = method;
        this.body = body;
        this.onSucess = onSucessCallBack;
        this.onFail = onFailCallback;
    }

    @Override
    protected String doInBackground(String... args) {
        
        // params comes from the execute() call: params[0] is the url.
       try {
            return downloadUrl("http://132.207.89." + suffix, method, body);//urls[0]);
        }
       catch (SocketTimeoutException e) {
           return "TIMEOUT";
       }
       catch (Exception e) {
           //Utilities.messageBox("Error while sending and receiving http response", e.getMessage());
           Utilities.printStackTrace(e);
           //Game.recoverFromError();
           return new String("ERROR " + e.getLocalizedMessage());

        }
    }
    // onPostExecute displays the results of the AsyncTask.
    @Override
    protected void onPostExecute(String result) {
        //appeler ce qui doit etre fait avec le resultat
        //Utilities.messageBox("Get result", );

        try {
            if (result.equals("TIMEOUT")) {
                new RequestTask(onSucess,null, receiveJSON, suffix, method, body).execute();
                return;
            }
            if (result != null) {
                if (result.contains("ERROR ")) throw new Exception(result);
                onSucess.runResponse(result);
            }
        }
        catch (Exception e) {

            Utilities.printStackTrace(e);
            Utilities.messageBox("Error handling the http response", e.getMessage());
            try {
                if (onFail != null){
                    onFail.runResponse("test");
                }
            } catch (JSONException e1) {
                e1.printStackTrace();
            }
            if (Utilities.currentActivity.getClass().getSimpleName().equals("ActivityGame")) {
                Game.recoverFromError();
            }
        }
    }


    private String downloadUrl(String myurl, String method, String body) throws Exception {
        InputStream is = null;

        URL url = new URL(myurl);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setReadTimeout(4000 /* milliseconds */);
        conn.setConnectTimeout(4000 /* milliseconds */);
        conn.setDoInput(true);
        //conn.setDoOutput(true);
        conn.setRequestMethod(method);
        Log.d("Methode  ", method);
        if (!body.equals("")) {

            conn.setRequestProperty("Content-Type", "application/json");
            conn.setRequestProperty("Accept", "application/json");
            OutputStreamWriter wr = new OutputStreamWriter(conn.getOutputStream());
            wr.write(ActivityCreateGame.password + "\r\n\r\n");

            wr.write(body);
            wr.flush();
        }
        else if (method.equals("POST")) {
                OutputStreamWriter wr = new OutputStreamWriter(conn.getOutputStream());
                wr.write(ActivityCreateGame.password  + "\r\n\r\n");
                wr.flush();
        }

        //conn.setRequestProperty("content-length", );
        // Starts the query

        conn.connect();

        int responseCode = conn.getResponseCode();
        String responseMessage = conn.getResponseMessage();
        if (responseCode != 200) throw new Exception(responseCode + " : " + responseMessage);
        is = conn.getInputStream();
        int len = conn.getContentLength();
        Log.d("Code " , String.valueOf(responseCode));
        Log.d("Content length", "" + len);
        Log.d("Response message", responseMessage);
        String contentAsString = "";
        if (receiveJSON) {
            contentAsString = readIt(is, len);//readInputStreamToString(conn);
        }

        conn.disconnect();
        return contentAsString;
    }


    // Reads an InputStream and converts it to a String.
    public String readIt(InputStream stream, int len) throws IOException, UnsupportedEncodingException {
        Reader reader = null;
        reader = new InputStreamReader(stream, "UTF-8");
        char[] buffer = new char[len];
        reader.read(buffer);
        return new String(buffer);
    }

}