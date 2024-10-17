

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"


#define  I2C_FREQ       100000
#define  I2C_GPIO_SDA   GPIO_NUM_17
#define  I2C_GPIO_SCL   GPIO_NUM_18
#define  I2C_PORT_NUM   I2C_NUM_1
#define  ACK_CHECK_EN    0x1
#define  ACK_CHECK_DES   0x0
#define  ACK_VAL         0x0
#define  NACK_VAL        0x1
#define GY_271_ADDRESS_WRITE   0X1A   //  0D+0
#define GY_271_ADDRESS_READ    0X1B  //   0D+1
  

uint8_t dataraw0,dataraw1,dataraw2,dataraw3,dataraw4,dataraw5,dataraw6,dataraw7,dataraw8;
int x,y,z;
static const char *TAG = "GY_271";



void Gy271_launch1(){

      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, GY_271_ADDRESS_WRITE , ACK_CHECK_EN  );
      i2c_master_write_byte(cmd, 0x0B , ACK_CHECK_EN );
      i2c_master_write_byte(cmd, 0x0B , ACK_CHECK_EN );
      i2c_master_stop(cmd);
      i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd);
      usleep(1000);

}

void Gy271_launch2(){



      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, GY_271_ADDRESS_WRITE , ACK_CHECK_EN  );
      i2c_master_write_byte(cmd, 0x09 , ACK_CHECK_EN );
      i2c_master_write_byte(cmd, 0x1D , ACK_CHECK_EN );
      i2c_master_stop(cmd);
      i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd);
      usleep(1000);  

}


void r (uint8_t *dataraw0,uint8_t *dataraw1,uint8_t *dataraw2,uint8_t *dataraw3,uint8_t *dataraw4,uint8_t *dataraw5,uint8_t *dataraw6,uint8_t *dataraw7,uint8_t *dataraw8 ){

      //uint8_t *dataraw[12] ;
      uint8_t x,y,z;
      i2c_cmd_handle_t cmd = i2c_cmd_link_create();
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, GY_271_ADDRESS_WRITE , ACK_CHECK_EN );
      i2c_master_write_byte(cmd, 0x00 , ACK_CHECK_EN );
      i2c_master_start(cmd);
      i2c_master_write_byte(cmd, GY_271_ADDRESS_READ  , ACK_CHECK_EN );


      i2c_master_read_byte(cmd, dataraw0 , 0);
      i2c_master_read_byte(cmd, dataraw1 , 0);
      i2c_master_read_byte(cmd, dataraw2 , 0);
      i2c_master_read_byte(cmd, dataraw3 , 0);
      i2c_master_read_byte(cmd, dataraw4 , 0);
      i2c_master_read_byte(cmd, dataraw5 , 0);
      i2c_master_read_byte(cmd, dataraw6 , 0);
      i2c_master_read_byte(cmd, dataraw7 , 0);
      i2c_master_read_byte(cmd, dataraw8 , 1);


      i2c_master_stop(cmd);
      i2c_master_cmd_begin(I2C_PORT_NUM, cmd, 1000 / portTICK_PERIOD_MS);
      i2c_cmd_link_delete(cmd);
      usleep(1000); 
    
     



}



    
    

void app_main(void)
{

i2c_config_t  i2c_conf ={


        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_GPIO_SDA,
        .scl_io_num = I2C_GPIO_SCL,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = I2C_FREQ,
		.clk_flags = 0,

        
    };

//___________________________________________________________________
i2c_param_config(I2C_PORT_NUM ,  &i2c_conf);
//___________________________________________________________________
i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0);
//___________________________________________________________________






Gy271_launch1();
Gy271_launch2();


while (1)
{

       r(&dataraw0,&dataraw1,&dataraw2,&dataraw3,&dataraw4,&dataraw5,&dataraw6,&dataraw7,&dataraw8);
       x = dataraw1 * 256 + dataraw0 ;
       y = dataraw3 * 256 + dataraw2 ;
       z = dataraw5 * 256 + dataraw4 ;


       if((x>>15) == 1)
       {
             x--;
             x = 65535 - x;
             x = x;
       }
       
       if((y>>15) == 1)
       {
             y--;
             y = 65535 - y;
             y = y;
       }

       if((z>>15) == 1)
       {
             z--;
             z = 65535 - z;
             z = z;
       }

       x = x >> 2; 
       y = y >> 2; 
       z = z >> 2; 
    
       ESP_LOGE(TAG, "Axis_X----->>> : %d\n",x);
       ESP_LOGE(TAG, "Axis_Y----->>> : %d\n",y);
       ESP_LOGE(TAG, "Axis_Z----->>> : %d\n",z);
      
       printf("_________________________________\n");

       vTaskDelay(1000 / portTICK_PERIOD_MS);



  
      
       

 }


}
