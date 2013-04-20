require 'yaml'
require 'sinatra'
require 'serialport'

class SerialButton

    def initialize file = "serial.yml"
        opt = YAML.load(IO.read(file))
        @port = SerialPort.new(opt['port'], opt['rate'], opt['data'], opt['stop'], SerialPort::NONE)
    end

    def press n
        @port.write n
    end

end

configure do
    @@serial = SerialButton.new
end

get '/' do
    erb :genius
end

get '/press/:button' do
    # mapping = { '0' => 48, '1' => 49,
    #             '2' => 50, '3' => 51}
    # chr = mapping[params[:button]]

    puts "O cara apertou #{params[:button]}"

    # if chr
    @@serial.press(params[:button])
    # end

    redirect '/'
end