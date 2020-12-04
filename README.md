# Practica4_Ethan_Alex
##subtitle
Para la practica 4 se pidio realizar un banco de filtros que se pueden aplicar a una señal de audio. La señal de audio es tomada por un codec que manda la señal por medio de I2S. Despues por medio de UART se decide que filtro se quiere aplicar. Se cuenta con un filtro pasa altas, un filtro pasa bajas y un filtro pasa banda. Despues de aplicar el filtro, se tiene que mandar la señal al codec para que este la reproduzca. 
Para poder usar los filtros se obtuvieron los coeficientes por medio de Matlab. En Matlab se diseño utilizando la funcion de fir, despues estos coeficientes obtenidos se guardaron en arreglos en el codigo de la practica, de tal manera que al llamar las funciones de CMSIS-DSP se pueda aplicar el filtro. 
Para la configuracion del code se utiliza el protocolo de I2C. Por medio de este protocolo se manda la configuracion para que el codec mande los datos y reciba los datos ya procesados. 
