// #include<Arduino.h>
#include<SimpleFOC.h>

MagneticSensorSPI sensor = MagneticSensorSPI(AS5048_SPI, 10);
BLDCMotor motor = BLDCMotor(11);
BLDCDriver3PWM driver = BLDCDriver3PWM(9, 5, 6, 8);
Commander command = Commander(Serial);

float target_angle = 1.15;
// float target_velocity = 0;
// float target_torque = 0;
float voltage_limit = 2;

class E6B2_CWZ6C_encoder
{
  public:
    static int rawCount;
    //static bool direction;
    static void read_rawCount();
    static void init();
    float voltage_transfer();
};
E6B2_CWZ6C_encoder S;
int E6B2_CWZ6C_encoder::rawCount = 0;
//bool E6B2_CWZ6C_encoder::direction = true;
void E6B2_CWZ6C_encoder::read_rawCount()
{
  if (digitalRead (3)==LOW) 
  {
    rawCount++;
  } 
  else
  {
    rawCount--;
  }
}
void E6B2_CWZ6C_encoder::init()
{
  attachInterrupt ( 0, read_rawCount, FALLING);
  DDRD &= (0<<2) | (0<<3);
  PORTD &= (1<<2) | (1<<3);
}
float E6B2_CWZ6C_encoder::voltage_transfer()
{
  //return (rawCount*(2*PI/2000));
  return (abs(rawCount) * (2 * PI / 2000));
}
//////////////////////////////// hàm gọi command ////////////////////////////////
/*
void doMotor(char* cmd) { command.motor(&motor, cmd); }
void doTarget(char* cmd)  // Trỏ giá trị từ serial tới target_angle
{
  command.scalar(&target_angle, cmd);
  motor.controller = MotionControlType::angle;
}

void doVelocity(char* cmd)  // Trỏ giá trị từ serial tới target_velocity
{
  command.scalar(&target_velocity, cmd);
  motor.controller = MotionControlType::velocity;
}

void doTorque(char* cmd)    // Trỏ giá trị từ serial tới target_torque
{
  command.scalar(&target_torque, cmd);
  motor.controller = MotionControlType::torque;
}

void reset_motor_voltage_limit(char* cmd) // Trỏ giá trị từ serial tới votage_limit
{
  command.scalar(&motor.voltage_limit, cmd);
  motor.init();
}

void change_voltage_limit(char* cmd)  // trỏ giá trị từ serial tới velocity_limit(PID)
{
  command.scalar(&motor.PID_velocity.limit, cmd);
}
*/
//////////////////////////////// hàm gọi command ////////////////////////////////

void setup() {
  Serial.begin(115200);
  S.init();
  //////////////////////////////// Bật debug ////////////////////////////////
  SimpleFOCDebug::enable(&Serial);
//////////////////////////////// Bật debug ////////////////////////////////

//////////////////////////////// động cơ-encoder-driver ////////////////////////////////
  sensor.init();
  motor.linkSensor(&sensor);
  driver.voltage_power_supply = 12;
  driver.init();
  motor.linkDriver(&driver);
//////////////////////////////// động cơ-encoder-driver ////////////////////////////////

//////////////////////////////// loại điều khiển động cơ ////////////////////////////////
  motor.foc_modulation = FOCModulationType::SinePWM;
  motor.controller = MotionControlType::angle;
//////////////////////////////// loại điều khiển động cơ ////////////////////////////////

//////////////////////////////// PID - điện áp - bộ lọc ////////////////////////////////
  motor.PID_velocity.P = 0.1f;
  motor.PID_velocity.I = 7;
  motor.PID_velocity.D = 0.0001;

  motor.voltage_limit = voltage_limit;  

  motor.LPF_velocity.Tf = 0.01f;
  motor.LPF_angle.Tf = 0.01f;

  motor.P_angle.P = 10;

  motor.velocity_limit = 10;
//////////////////////////////// PID - điện áp - bộ lọc ////////////////////////////////
  
  motor.useMonitoring(Serial);
  //motor.monitor_variables = _MON_TARGET | _MON_VEL | _MON_ANGLE; 
  //motor.monitor_downsample = 100; // default 10
  motor.init();
  motor.initFOC();

//////////////////////////////// Gọi command serial ////////////////////////////////
  // command.add('G', doTarget, "target angle"); // G + float
  // command.add('V', doVelocity, "target velocity"); // V + float
  // command.add('L', doTorque, "target torque");  // L + float
  // command.add('R', reset_motor_voltage_limit, " motor.voltage_limit");  // V + float
  // command.add('S', change_voltage_limit, "motor.PID_velocity.limit");  // S + float
  // command.add('M',doMotor,"motor");fdsaasddfff
//////////////////////////////// Gọi command serial ////////////////////////////////
  _delay(1000);
}

void loop() {
  //Serial.println(S.voltage_transfer());
  motor.loopFOC();
  motor.move(target_angle);
  //float voltage = S.rawCount*(2*PI/2000);
  //motor.PID_velocity.limit = S.voltage_transfer();
  // if      (motor.controller == MotionControlType::angle)    motor.move(target_angle);
  // else if (motor.controller == MotionControlType::velocity) motor.move(target_velocity);
  // else if (motor.controller == MotionControlType::torque)   motor.move(target_torque);
  //motor.monitor();
//////////////////////////////// Vòng loop điều khiển motor ////////////////////////////////

//////////////////////////////// Kiểm tra dòng ////////////////////////////////
  // float Pha_a = motor.voltage.q * sin(motor.electrical_angle);
  // float Pha_b = motor.voltage.q * sin(motor.electrical_angle - _2PI/3);
  // float Pha_c = motor.voltage.q * sin(motor.electrical_angle + _2PI/3);
  // float D=motor.shaft_velocity; 
  //int rawCountA = sensor.getRawCount();
  // Serial.print(">Phase_a: ");Serial.println(Pha_a);
  // Serial.print(">Phase_b: ");Serial.println(Pha_b);
  // Serial.print(">Phase_c: ");Serial.println(Pha_c);
  // Serial.print(">Velocity: ");Serial.println(D); // Kiểm tra vận tốc
  // Serial.print(">Angle: ");Serial.println(motor.shaft_angle);
  // Serial.print(">Velocity: ");Serial.println(motor.shaft_velocity);
  // Serial.print(">Voltage: ");Serial.println(motor.voltage.q);
  Serial.print(">Encoder A: ");Serial.println(sensor.getRawCount());
  Serial.print(">Encoder B: ");Serial.println(S.rawCount);
  Serial.print(">input_voltage: ");Serial.println(S.voltage_transfer());
  Serial.print(">motor_max_limit_voltage: ");Serial.println(motor.voltage.q);
//////////////////////////////// Kiểm tra dòng ////////////////////////////////
  command.run();
}