package com.punchthrough.blestarterappandroid

import android.annotation.SuppressLint
import android.bluetooth.BluetoothDevice
import android.os.Bundle
import android.view.MotionEvent
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import com.punchthrough.blestarterappandroid.ble.ConnectionManager
import timber.log.Timber
import java.util.UUID
import kotlin.math.cos
import kotlin.math.sin


class Controller : AppCompatActivity() {

    // 1. Define the device variable at the top
    private lateinit var device: BluetoothDevice

    @SuppressLint("ClickableViewAccessibility", "MissingInflatedId")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_controller)

        // 2. Get the device passed from the previous activity
        device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE)
            ?: error("No BluetoothDevice found in intent")

        // 3. Link your UI Buttons (ensure these IDs match your XML)
       // val btnLedOn = findViewById<Button>(R.id.button_led_on)

        //EXAMPLE
        val btnControl = findViewById<Button>(R.id.button_led_on)
        btnControl.setOnTouchListener { view, event ->
            when (event.action) {
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("1")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("0")
                    true
                }
                else -> false
            }
        }

        val leftForwardBtn = findViewById<Button>(R.id.button_led_forwardL)
        leftForwardBtn.setOnTouchListener { view, event ->
            when (event.action){
                MotionEvent.ACTION_DOWN -> {
                    sendToArduino("FLD")
                    true
                }
                MotionEvent.ACTION_UP, MotionEvent.ACTION_CANCEL -> {
                    sendToArduino("FLU")
                    true
                }
                else -> false
            }
        }

        val joystickL = findViewById<JoystickView>(R.id.joystick_custom)
        joystickL.onMoveListener = { angle, strength ->
            sendJoystickPosition(angle, strength,"LJ")
        }

        val joystickR = findViewById<JoystickView>(R.id.joystick_right)
        joystickR.onMoveListener = { angle, strength ->
            sendJoystickPosition(angle, strength,"RJ")
        }

    }

    private fun sendJoystickPosition(angle: Int, strength: Int, joyStickName: String) {
        val rad = Math.toRadians(angle.toDouble())
        val nx = cos(rad) * strength / 100.0
        val ny = -sin(rad) * strength / 100.0
        val bx = ((nx + 1.0) * 127.5).toInt().coerceIn(0, 255)-127
        val by = ((ny + 1.0) * 127.5).toInt().coerceIn(0, 255)-127
        sendToArduino("$joyStickName,$bx,$by")
    }

    private fun sendToArduino(command: String) {
        // 1. Get the list of services from the ConnectionManager
        val services = ConnectionManager.servicesOnDevice(device)
        val output = command + "\n"

        // 2. Find the specific HM-10 Serial Service
        val service = services?.find { it.uuid == UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb") }

        // 3. Find the specific HM-10 Serial Characteristic
        val characteristic = service?.getCharacteristic(UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb"))

        characteristic?.let { char ->
            val bytes = output.toByteArray(Charsets.UTF_8)
            // 4. Send it! The Manager handles the queueing.
            ConnectionManager.writeCharacteristic(device, char, bytes)
        } ?: run {
            Timber.e("HM-10 Serial characteristic not found!")
        }
    }
}
