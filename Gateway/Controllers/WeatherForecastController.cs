using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

using System.Text;
using Microsoft.AspNetCore.Connections;
using RabbitMQ.Client;

namespace Gatewayy.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class WeatherForecastController : ControllerBase
    {
        [HttpGet]
        public string Getstart ()
        {
            Guid guid = Guid.NewGuid();
            return $"gateway url example https://localhost:44350/weatherforecast/{guid}";
        }
        // https://localhost:44350/weatherforecast/5
        [HttpGet("{id}")]
        public FileStreamResult GetbranchofficesDto(string id)
        {
            string ruta = "esta es la ruta id:" + id;


            var factory = new ConnectionFactory() { HostName = "localhost" };
            using (var connection = factory.CreateConnection())
            using (var channel = connection.CreateModel())
            {
                channel.QueueDeclare(queue: "libro-queue",
                                     durable: false,
                                     exclusive: false,
                                     autoDelete: false,
                                     arguments: null);


                string message = id;
                var body = Encoding.UTF8.GetBytes(message);

                channel.BasicPublish(exchange: "",
                                     routingKey: "libro-queue",
                                     basicProperties: null,
                                     body: body);
                Console.WriteLine(" [x] Sent {0}", message);
            }


            var byteArray = Encoding.ASCII.GetBytes(id);
            var stream = new MemoryStream(byteArray);
            Guid guid = Guid.NewGuid();

            return File(stream, "text/plain", $"{guid}.txt");
        }
    }
}
