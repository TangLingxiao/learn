# -*- coding: utf-8 -*-
import scrapy


class Wenku8Spider(scrapy.Spider):
    name = 'wenku8'
    allowed_domains = ['wenku8.com']
    start_urls = ['http://wenku8.com/']

    def parse(self, response):
        pass
