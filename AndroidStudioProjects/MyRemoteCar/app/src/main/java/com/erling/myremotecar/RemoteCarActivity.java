package com.erling.myremotecar;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
//import com.zerokol.views.JoystickView;
//import com.zerokol.views.JoystickView.OnJoystickMoveListener;
import android.app.Activity;
import android.widget.TextView;


public class RemoteCarActivity extends AppCompatActivity {
    private TextView angleTextView;
    private TextView powerTextView;
    private TextView directionTextView;
    // Importing also other views
//    private JoystickView joystick;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_remote_car);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
//        setSupportActionBar(toolbar);

//        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
//        fab.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View view) {
//                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
//                        .setAction("Action", null).show();
//            }
//        });
        angleTextView = (TextView) findViewById(R.id.angleTextView);
        powerTextView = (TextView) findViewById(R.id.powerTextView);
        directionTextView = (TextView) findViewById(R.id.directionTextView);
        //Referencing also other views
//        joystick = (JoystickView) findViewById(R.id.joystickView);

        //Event listener that always returns the variation of the angle in degrees, motion power in percentage and direction of movement
//        joystick.setOnJoystickMoveListener(new OnJoystickMoveListener() {
//
//            @Override
//            public void onValueChanged(int angle, int power, int direction) {
//                // TODO Auto-generated method stub
//                angleTextView.setText(" " + String.valueOf(angle) + "°");
//                powerTextView.setText(" " + String.valueOf(power) + "%");
//                switch (direction) {
//                    case JoystickView.FRONT:
//                        directionTextView.setText(R.string.front_lab);
//                        break;
////                    case JoystickView.FRONT_RIGHT:
////                        directionTextView.setText(R.string.front_right_lab);
////                        break;
////                    case JoystickView.RIGHT:
////                        directionTextView.setText(R.string.right_lab);
////                        break;
////                    case JoystickView.RIGHT_BOTTOM:
////                        directionTextView.setText(R.string.right_bottom_lab);
////                        break;
////                    case JoystickView.BOTTOM:
////                        directionTextView.setText(R.string.bottom_lab);
////                        break;
////                    case JoystickView.BOTTOM_LEFT:
////                        directionTextView.setText(R.string.bottom_left_lab);
////                        break;
////                    case JoystickView.LEFT:
////                        directionTextView.setText(R.string.left_lab);
////                        break;
////                    case JoystickView.LEFT_FRONT:
////                        directionTextView.setText(R.string.left_front_lab);
////                        break;
//                    default:
//                        directionTextView.setText(R.string.center_lab);
//                }
//            }
//        }, JoystickView.DEFAULT_LOOP_INTERVAL);


}

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_remote_car, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
