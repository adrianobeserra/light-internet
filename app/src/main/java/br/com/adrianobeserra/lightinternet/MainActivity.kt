package br.com.adrianobeserra.lightinternet

import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.widget.ImageButton
import android.widget.TextView
import android.widget.Toast
import io.reactivex.schedulers.Schedulers.io
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.MqttMessage
import rx.mqtt.android.RxMqtt
import rx.mqtt.android.RxMqtt.*

class MainActivity : AppCompatActivity() {

    private var lampadaLigadaMQTT = "D"
    private val topic = "lightwifi/1"
    private val broker = "tcp://broker.mqttdashboard.com:1883"
    private val mqttAndroidClient: MqttAndroidClient by lazy {
        RxMqtt.client(applicationContext, broker)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val btLampada = findViewById<ImageButton>(R.id.imageButton)
        val lblStatus = findViewById<TextView>(R.id.lbl_status)

        Thread {
            try {
                val subscribe = message(
                        mqttAndroidClient,
                        topic
                ).subscribe { it: MqttMessage ->
                    lampadaLigadaMQTT = it.toString()
                    System.out.println(lampadaLigadaMQTT)
                    runOnUiThread{
                        if (lampadaLigadaMQTT.equals("L")) {
                            btLampada.setImageResource(R.drawable.ic_light_on)
                        } else {
                            btLampada.setImageResource(R.drawable.ic_light_off)
                        }
                        toast(lampadaLigadaMQTT)
                        lblStatus.setText(getString(R.string.conexao_sucesso))
                        lblStatus.setTextColor(getResources().getColor(R.color.colorSuccess))
                    }
                }
            }
            catch (e: Exception) {
                runOnUiThread{
                    val erro = getString(R.string.conexao_erro)
                    lblStatus.setText("$erro  $broker")
                    lblStatus.setTextColor(getResources().getColor(R.color.colorAccent))
                    toast("$erro  $broker")
                }
            }

        }.start()

        btLampada.setOnClickListener {
            val charset = Charsets.UTF_8
            if (lampadaLigadaMQTT.equals("L")) {
                lampadaLigadaMQTT = "D"
                btLampada.setImageResource(R.drawable.ic_light_off)

            } else {
                lampadaLigadaMQTT = "L"
                btLampada.setImageResource(R.drawable.ic_light_on)
            }
            mqttAndroidClient.publish(topic, lampadaLigadaMQTT.toByteArray(charset), 0, true)
            toast(lampadaLigadaMQTT)
        }
    }

    fun callbBackSubscription(mensagem: MqttMessage) {
        lampadaLigadaMQTT = mensagem.toString()
        System.out.print("Teste ${mensagem}")
    }

    fun toast(message: String, length: Int = Toast.LENGTH_SHORT) {
        if (message.equals("L")) {
            Toast.makeText(this, "Lâmpada Ligada", length).show()
        } else if (message.equals("D")) {
            Toast.makeText(this, "Lâmpada Desligada", length).show()
        } else {
            Toast.makeText(this, message, length).show()
        }

    }

}
